#pragma once

#include "space.hxx"
#include "player.hxx"
#include "ghost.hxx"
#include "game_config.hxx"
#include "maze.hxx"

#include <ge211.hxx>

enum GameState {
    ready,
    running,
    gameover
};

class Model
{
public:
    explicit Model(Game_config const& config = Game_config());

    void on_frame(double dt);

    // Move the pacman
    void go_up();
    void go_down();
    void go_left();
    void go_right();

    // what to do when pacman dies
    void killPacman();

    // when pacman eats a power pellet, increase the blue mode counter
    void increaseBlueModeCounter();

    // proceed to the next level
    void nextLevel();

    // save high score
    void saveIfHighScore();

    // restart from level 1
    void restart();

    // member variables

    Game_config const config;
    Player pacman;

    Ghost ghosts[4];

    Maze maze;

    int score;
    int high_score;

    int lives;

    int level;

    // counts the number of seconds left till ghosts in blue mode become normal again
    float blue_mode_counter;

    // counts the number of consumeables on the maze consumed
    int consumed_count;

    // total number of consumeables on the maze
    int total_consumeables;

    GameState state;
};
