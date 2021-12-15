
#include "player.hxx"
#include "maze.hxx"

#include <ge211.hxx>
#include <iostream>

Player::Player(int radius, Position position, double velocity, ge211::Dims<int> direction)
        : radius(radius),
          position (position),
          velocity(velocity),
          direction (direction),
          next_direction {0,0}
{}

bool
Player::hits_wall(Maze const& maze) const {
    int x = this->position.x, y = this->position.y;
    float x_offset = this->position.x - x, y_offset = this->position.y - y;

    //for warp tunnels
    if (x + 1 == MAZE_WIDTH || x < 0) {
        return !(y == 9 && y_offset == 0);
    }

    if ( maze[y * MAZE_WIDTH + x] == Space::wall
    || (x_offset > 0 && maze[y * MAZE_WIDTH + x + 1] == Space::wall)
    || (y_offset > 0 && maze[(y+1) * MAZE_WIDTH + x] == Space::wall)
    || (x_offset > 0 && y_offset > 0 &&  maze[(y+1) * MAZE_WIDTH + x+1] == Space::wall)
    ) {
        return true;
    }
    return false;
}

int
Player::get_touched_dot(Maze const& maze) const {
    int x = this->position.x, y = this->position.y;
    x = this->position.x - x > 0.5 ? x+1 : x;
    y = this->position.y - y > 0.5 ? y+1 : y;
    
    if (maze[y*MAZE_WIDTH + x] == Space::dot) {
        return y*MAZE_WIDTH + x;
    }
    return -1;
}

int
Player::get_touched_pellet(Maze const& maze) const {
    int x = this->position.x, y = this->position.y;
    x = this->position.x - x > 0.5 ? x+1 : x;
    y = this->position.y - y > 0.5 ? y+1 : y;
    
    if (maze[y*MAZE_WIDTH + x] == Space::pellet) {
        return y*MAZE_WIDTH + x;
    }
    return -1;
}

void
Player::stick_to_wall(Maze const& maze, float dt) {
    int x = this->position.x, y = this->position.y;
    float limit = this->velocity * dt;
    
    if ( this->position.x - x < limit ) {
        this->position.x = x;
    }
    if ( this->position.y - y < limit ) {
        this->position.y = y;
    }
    if ( x+1 - this->position.x < limit ) {
        this->position.x = x+1;
    }
    if ( y+1 - this->position.y < limit ) {
        this->position.y = y+1;
    }
}

// `this` is a `const Player*`, and you can create a copy of a pacman
// with the copy constructor. So to get a new `Player` to return, you can write
//
//     Player result(*this);
//
Player
Player::next(double dt) const
{
    Player result(*this);
    result.position += result.direction * result.velocity * dt;

    //For warp tunnels
    if ( result.position.x + 1 <= 0 )
        result.position.x += MAZE_WIDTH + 1;
    if ( result.position.x >= MAZE_WIDTH )
        result.position.x -= MAZE_WIDTH + 1;
    
    return result;
}

bool
Player::can_change_direction(Maze const& maze, double dt) const {
    if (this->next_direction.width == 0 && this->next_direction.height == 0)
        return false;
    Player next_pacman(*this);
    next_pacman.stick_to_wall(maze, dt);
    next_pacman.direction = next_pacman.next_direction;
    next_pacman = next_pacman.next(dt);
    return !next_pacman.hits_wall(maze);
}

void
Player::change_direction() {
    this->direction = this->next_direction;
    this->next_direction = {0,0};
}

bool
operator==(Player const& a, Player const& b)
{
    return a.radius == b.radius
            && a.velocity == b.velocity
            && a.position == b.position
            && a.direction == b.direction;
}

bool
operator!=(Player const& a, Player const& b)
{
    return !(a == b);
}

std::ostream&
operator<<(std::ostream& o, Player const& pacman)
{
    o << "Player{";
    o << "\n\tposition: " << pacman.position
        << ",\n\tdirection: " << pacman.direction
        << ",\n\tradius: " << pacman.radius;
    return o << "\n}";
}
