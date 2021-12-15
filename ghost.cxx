
#include "ghost.hxx"
#include <vector>

/// Find square of the distance between two positions
float dist2(Position a, Position b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx*dx + dy*dy;
}

bool equal_coords(float a, float b) {
    float error = 0.2;
    float diff = a - b;
    return diff < error && diff > -error;
}

Direction oppositeDirectionOf(Direction const& dir) {
    Direction result = Direction(dir);
    if (dir.height != 0) result.height = -dir.height;
    else result.width = -dir.width;
    return result;
}

int
Ghost::eval_direction_(Direction const& dir, Position const& target) {
    Position new_pos = {int(this->position.x) + dir.width, int(this->position.y) + dir.height};
    float dist = dist2(new_pos, target)*10;
    int score = 0;
    switch (this->mode) {
    case GhostMode::normal:
    case GhostMode::white:
        //Prefer to go towards the target
        score = -dist;
        break;
    case GhostMode::blue:
        // prefer to go away from the target
        score = dist;
        break;
    }
    return score;
}

Ghost::Ghost(int radius, Position position, double velocity, ge211::Dims<int> direction)
        : Player(radius, position, velocity, direction), mode(GhostMode::normal)
{}

Ghost::Ghost(Player const& player, const GhostMode mode) : Player(player), mode(mode) {
}

void
Ghost::think(Maze const& maze, float dt, Position const& target) {
    Direction dirs[4] = {{-1,0},{0,-1},{1,0},{0,1}};
    
    float direction_scores[4] = {0,}; //maps to above array, corresponding direction scores
    char available_directions[4] = {0,}; //maps to above array, 1 -> corresponding direction avaialble
    int available_count = 0, best_dir = -1;
    bool current_available = false;
    float highest_score = -9999;

    Direction opposite_direction = oppositeDirectionOf(this->direction);

    Ghost ghost = Ghost(*this);
    for(int i = 0; i < 4 ; i++) {
        ghost.next_direction = dirs[i];

        if (ghost.can_change_direction(maze, dt)) {
            if(dirs[i] != opposite_direction){
                direction_scores[i] = this->eval_direction_(dirs[i], target);
                if (direction_scores[i] > highest_score) {
                    highest_score = direction_scores[i];
                    best_dir = i;
                }
                if (this->direction == dirs[i])
                    current_available = true;
            }
            available_directions[i] = 1;
            available_count ++;
        }
    }

    if (current_available && this->keep_direction_time_ > 0) {
        this->keep_direction_time_ -= dt;
        return;
    }

    if (best_dir != -1) {
        // found a good direction to follow
        this->change_direction_to_(dirs[best_dir], maze);
    } else if (available_count != 0) {
        // don't have a good direction, so select a random one among available
        ge211::Random_source<int> die{1, available_count};
        int select_dir_n = die.next();
        for(int i =0;i<4;i++){
            if(available_directions[i])
                select_dir_n --;
            if(!select_dir_n) {
                this->change_direction_to_(dirs[i], maze);
                break;
            }
        }
    } else {
        this->change_direction_to_(oppositeDirectionOf(this->direction), maze);
    }
    this->keep_direction_time_ = dt*4;
}

void
Ghost::change_direction_to_(Direction const& dir, Maze const& maze) {
    this->next_direction = dir;
    this->change_direction();
}

Ghost
Ghost::next(float dt) {
    Player player = Player::next(dt);
    //warp tunnel
    if (this->position.x - player.position.x > 2 || player.position.x - this->position.x > 2) {
        this->velocity -= this->velocity/10;
    }
    return Ghost(player, this->mode);
}

bool
Ghost::touches_pacman(Player const& pacman) {
    float d = 1;
    
    return (dist2(this->position, pacman.position) <= d);
}