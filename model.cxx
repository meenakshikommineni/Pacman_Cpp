#include "model.hxx"

#include <fstream>

const int DOT_SCORE = 10;
const int PELLET_SCORE = 50;
const int GHOST_SCORE = 200;

int load_highscore_if_exists(std::string filepath) {
    std::ifstream score_file;
    score_file.open(filepath);
    if(!score_file) {
        return 0;
    }
    int score;
    score_file >> score; //todo: maybe add player name too
    score_file.close();
    return score;
}

Maze
init_maze(const std::string maze_str) {

    std::array<Space, MAZE_HEIGHT*MAZE_WIDTH> maze;
    for(auto i = 0;i<MAZE_HEIGHT*MAZE_WIDTH;i++) {
        if (maze_str[i] == 'w') {
            maze[i] = Space::wall;
        } else if (maze_str[i] == '.') {
            maze[i] = Space::dot;
        } else if (maze_str[i] == 'p') {
            maze[i] = Space::pellet;
        } else {
            maze[i] = Space::empty;
        }
    }
    return maze;
}

int count_consumeables(Maze const& maze) {
    int count = 0;
    for (int i=0;i<MAZE_WIDTH*MAZE_HEIGHT; i++) {
        if (maze[i] == Space::pellet || maze[i] == Space::dot)
            count ++;
    }
    return count;
}

bool
is_spawn_position(Position const& pos) {
    Position spawn_positions[] = {{8,9},{9,9},{10,9},{9,8},{9,7}};
    Position floored_pos = Position(pos);
    floored_pos.x = int(pos.x);
    floored_pos.y = int(pos.y);
    for(int i=0;i<5;i++) {
        if (floored_pos == spawn_positions[i])
            return true;
    }
    return false;
}

Model::Model(Game_config const& config)
        : config(config),
         pacman(config.pacman_radius,
            config.pacman_position_0,
            config.pacman_velocity_0,
            {-1,0}),
        ghosts{Ghost(config.ghost_radius,
            config.ghost_position_0[0],
            config.ghost_velocity_0,
            {-1,0}),Ghost(config.ghost_radius,
            config.ghost_position_0[1],
            config.ghost_velocity_0,
            {-1,0}),Ghost(config.ghost_radius,
            config.ghost_position_0[2],
            config.ghost_velocity_0,
            {-1,0}),Ghost(config.ghost_radius,
            config.ghost_position_0[3],
            config.ghost_velocity_0,
            {-1,0})},
        maze(init_maze(config.maze_str)),
        score(0),
        high_score(load_highscore_if_exists(config.high_score_file)),
        lives(3),
        level(1),
        blue_mode_counter(0),
        consumed_count(0),
        total_consumeables(count_consumeables(this->maze)),
        state(GameState::ready)
{
}

void
Model::on_frame(double dt)
{
    if (state == GameState::running) {
        Player next_pacman = this->pacman.next(dt);

        bool update_pacman = true;

        if (this->pacman.can_change_direction(this->maze, dt)) {
            this->pacman.stick_to_wall(this->maze, dt);
            this->pacman.change_direction();
        }

        if (next_pacman.hits_wall(this->maze)) {
            this->pacman.stick_to_wall(this->maze, dt);
            update_pacman = false;
        }

        int touched_consumeable;
        if ( (touched_consumeable = next_pacman.get_touched_dot(this->maze)) != -1) {
            this->maze[touched_consumeable] = Space::empty;
            score += DOT_SCORE;
            this->consumed_count ++;
        } else if ( (touched_consumeable = next_pacman.get_touched_pellet(this->maze)) != -1) {
            this->maze[touched_consumeable] = Space::empty;
            this->increaseBlueModeCounter();
            score += PELLET_SCORE;
            this->consumed_count ++;
        }

        if (this->consumed_count >= this->total_consumeables) {
            this->nextLevel();
        }
        
        for(int i = 0 ;i < 4; i++) {
            Position target = {0,0};
            if (this->ghosts[i].mode == GhostMode::white) {
                target = this->config.ghost_position_0[i];
                if (is_spawn_position(this->ghosts[i].position) ) {
                    this->ghosts[i].mode = GhostMode::normal;
                }
            } else {
                if (is_spawn_position(this->ghosts[i].position)) {
                    target = {11, 6};
                    ge211::Random_source<int> coin{0,1};
                    target.x = coin.next()?8:10;
                    // std::cout << target << std::endl;
                } else if (i == 0) //chase pacman directly
                    target = this->pacman.position;
                else if(i == 1) //chase 4 tiles ahead of pacman
                    target =
                        {this->pacman.position.x + 4*this->pacman.direction.width, this->pacman.position.y + 4*this->pacman.direction.height};
                else if( i == 2) {// chase 2 tiles ahead of pacman when at least 30 dots consumed
                    Position two_ahead = {this->pacman.position.x + 2*this->pacman.direction.width, this->pacman.position.y + 2*this->pacman.direction.height};
                    Position maze_corner = {10,19};
                    target = this->consumed_count > 30?
                        two_ahead
                        :maze_corner;
                }
                else if(i==3) {
                    ge211::Random_source<int> coin{0,1};
                    target = coin.next()? this->pacman.position
                        :this->config.ghost_position_0[i];
                }
            }
            this->ghosts[i].think(this->maze, dt, target);

            if (this->ghosts[i].touches_pacman(this->pacman)) {
                if (this->ghosts[i].mode == GhostMode::normal)
                    this->killPacman();
                else if(this->ghosts[i].mode == GhostMode::blue) {
                    this->ghosts[i].mode = GhostMode::white;
                    score += GHOST_SCORE;
                }
            }

            if (this->ghosts[i].mode == GhostMode::blue) {
                this->blue_mode_counter -= dt;
                if (this->blue_mode_counter <= 0) {
                    this->ghosts[i].mode = GhostMode::normal;
                }
            }
            
            this->ghosts[i] = this->ghosts[i].next(dt);
        }

        if(update_pacman)
            this->pacman = this->pacman.next(dt);
    }
}

void
Model::go_up()
{
    Direction dir = {0,-1};
    this->pacman.next_direction = dir;
}

void
Model::go_left()
{
    Direction dir = {-1, 0};
    this->pacman.next_direction = dir;
}

void
Model::go_right()
{
    Direction dir = {1, 0};
    this->pacman.next_direction = dir;
}

void
Model::go_down()
{
    Direction dir = {0,1};
    this->pacman.next_direction = dir;
}

void
Model::killPacman() {
    this->lives --;
    
    if (this->lives != 0) {

        //reset pacman and ghost positions
        this->pacman.position = this->config.pacman_position_0;
        this->pacman.direction = {-1,0};
        for(int i=0; i < 4; i++) {
            this->ghosts[i].position = this->config.ghost_position_0[i];
            this->ghosts[i].mode = GhostMode::normal;
            this->ghosts[i].direction = {0,0};
        }
        this->state = GameState::ready;
    } else {
        this->state = GameState::gameover;
        this->saveIfHighScore();
    }

    this->blue_mode_counter = 0;
}

void
Model::increaseBlueModeCounter() {
    this->blue_mode_counter += this->level < 15? 30 - this->level : 10;
    for(int i=0;i<4;i++) {
        this->ghosts[i].mode = GhostMode::blue;
    }
}

void Model::nextLevel() {
    this->level ++;

    this->lives ++;
    killPacman(); //use this to reset positions and set game state

    this->pacman.velocity = this->level*this->config.pacman_velocity_0;

    for(int i=0;i<4;i++) {
        this->ghosts[i].velocity = this->level * this->config.ghost_velocity_0;
    }

    this->maze = init_maze(this->config.maze_str);
    this->consumed_count = 0;
    this->saveIfHighScore();
}

void Model::restart() {
    this->level = 0;
    this->lives = 3;
    this->nextLevel();
    this->state = GameState::ready;
    this->score = 0;
}

void
Model::saveIfHighScore() {
    if(this->score > this->high_score) {
        this->high_score = this->score;
        std::ofstream score_file;
        score_file.open(this->config.high_score_file);
        score_file << this->score;
        score_file.close();
    }
}