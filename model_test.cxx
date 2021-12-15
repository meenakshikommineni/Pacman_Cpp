#include "model.hxx"
#include <catch.hxx>

/// to compare float position
bool equal_coords(Position a, Position b) {
    float error = 0.01;
    float dx = a.x - b.x, dy = a.y - b.y;
    return dx < error && dx > -error && dy < error && dy > -error;
}

TEST_CASE("GHOST EATS PACMAN") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    CHECK(model.lives == 3);

    model.pacman.position = {1,1};
    model.ghosts[0].position = model.pacman.position; // ghost touch pacman

    model.on_frame(0.1);

    CHECK(model.lives == 2);
    CHECK(model.pacman.position == config.pacman_position_0); // pacman position reset
    CHECK(model.ghosts[0].position == config.ghost_position_0[0]); // ghosts position reset
}

TEST_CASE("PACMAN TOUCHES A BLUE GHOST") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    model.ghosts[0].mode = GhostMode::blue;
    model.ghosts[0].position = model.pacman.position; // ghost touch pacman

    CHECK(model.score == 0);

    model.on_frame(0.1);

    CHECK(model.score == 200); //200 points to consume a blue ghost
    CHECK(model.ghosts[0].mode == GhostMode::white); // ghost turned to white mode
}

TEST_CASE("PACMAN TOUCHES A WHITE GHOST") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    model.ghosts[0].mode = GhostMode::white;
    model.ghosts[0].position = model.pacman.position; // ghost touch pacman

    CHECK(model.score == 0);
    CHECK(model.lives == 3);

    model.on_frame(0.1);

    CHECK(model.score == 0); //no score change
    CHECK(model.lives == 3); // pacman stays alive
    CHECK(model.ghosts[0].mode == GhostMode::white); // ghost still white mode
}

TEST_CASE("PACMAN EATS PELLET") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    for(int i=0;i<4;i++) {
        CHECK(model.ghosts[i].mode == GhostMode::normal);
    }

    CHECK(model.score == 0);

    model.pacman.position = {1,2};

    model.on_frame(0.1);

    for(int i=0;i<4;i++) {
        CHECK(model.ghosts[i].mode == GhostMode::blue); // ghosts turn blue
    }

    CHECK(model.score == 50); //50 points to consume a pellet
}

TEST_CASE("PACMAN EATS DOT") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    for(int i=0;i<4;i++) {
        CHECK(model.ghosts[i].mode == GhostMode::normal);
    }

    CHECK(model.score == 0);

    model.pacman.position = {1,1};

    model.on_frame(0.1);

    for(int i=0;i<4;i++) {
        CHECK(model.ghosts[i].mode == GhostMode::normal); // ghosts no change
    }

    CHECK(model.score == 10); //10 points to consume a dot
}

TEST_CASE("PACMAN TRYING TO GO THROUGH A WALL") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    model.pacman.position = {1,1};
    model.pacman.direction = {-1,0};

    model.on_frame(0.1);

    CHECK(model.pacman.position == Position{1,1}); // no change since attempting to go through a wall
    model.on_frame(0.1);
    CHECK(model.pacman.position == Position{1,1}); // no change since attempting to go through a wall
    model.on_frame(0.1);
    CHECK(model.pacman.position == Position{1,1}); // no change since attempting to go through a wall
}

TEST_CASE("PACMAN TRYING TO CHANGE DIRECTION TO GO IN A WALL") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    model.pacman.position = {1,1};
    model.pacman.direction = {0,1}; // going down

    model.on_frame(0.1);

    model.go_left(); // try to go through wall

    model.on_frame(0.1);

    CHECK(model.pacman.direction == Direction{0,1}); //same direction since can't go through wall
}

TEST_CASE("PACMAN GOING THOUGH WARP TUNNEL") {
    Game_config config;
    Model model(config);

    model.state = GameState::running; // start the game

    model.pacman.position = {0,9}; //edge of maze i.e. start of left warp tunnel
    model.pacman.direction = {-1,0}; // going left

    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);

    CHECK(model.pacman.direction == Direction{-1,0}); //same direction
    CHECK(equal_coords(model.pacman.position, Position{-1,9})); // going outside the maze

    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);
    model.on_frame(0.1);

    CHECK(model.pacman.direction == Direction{-1,0}); //same direction
    CHECK(equal_coords(model.pacman.position, Position{MAZE_WIDTH - 1,9})); // coming from the other side
}