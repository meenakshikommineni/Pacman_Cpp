

// Defines a struct for modeling pacman.

#pragma once

#include "player.hxx"

#include <ge211.hxx>

enum GhostMode {
    white,
    blue,
    normal
};

/// Position of the pacman
///   struct Position
///   {
///       float x;   // pixel distance from left edge of window
///       float y;   // pixel distance from top edge of window
///   };
using Position = ge211::Posn<float>;

///   Direction the pacman is facing in
using Direction = ge211::Dims<float>;


/// A ghost is an automated player
/// Ghost's moving pattern is as follows:
/// If it not moving, then it choses a random direction which brings it closer to Pacman
/// If it _can_ change direction, then it choses a random direction which brings it closer to Pacman
struct Ghost : Player {

    Ghost(int, Position, double, ge211::geometry::Dims<int>);

    /// Thinks about whether to change direction and changes it if necessary
    void think(Maze const&, float dt, Position const&);

    /// next position of the ghost
    Ghost next(float dt);

    // check if ghost touches a pacman
    bool touches_pacman(Player const&);

    GhostMode mode;


private:
    void change_direction_to_(Direction const&, Maze const&);
    int eval_direction_(Direction const&, Position const&);

    Ghost(Player const&, const GhostMode);

    float keep_direction_time_; // keep the direction same for some time
};