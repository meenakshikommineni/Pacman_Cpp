
#pragma once

#include <ge211.hxx>


struct Game_config
{
    // Constructs an instance with the default parameters.
    Game_config();

    // The dimensions of the whole window:
    ge211::Dims<int> scene_dims;

    // size of 1 block of a maze in pixels
    int block_size;

    // The pacman's radius
    int pacman_radius;

    // pacman's initial velocity and position
    double pacman_velocity_0;
    ge211::Posn<float> pacman_position_0;

    // The ghost's radius
    int ghost_radius;

    // ghost's initial velocity and position
    double ghost_velocity_0;

    ge211::Posn<float> ghost_position_0[4];

    // Number of pixels from top of screen to top of score:
    int top_margin;

    // Number of pixels from sides of screen to sides of brick formation:
    int side_margin;

    // Number of pixels from bottom of screen to bottom of paddle.
    int bottom_margin;

    // The maze in string format
    std::string maze_str;

    // The file path where high score will be stored
    std::string high_score_file;

};

