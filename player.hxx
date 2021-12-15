

// Defines a struct for modeling pacman.

#pragma once

#include "maze.hxx"

#include <ge211.hxx>

/// Position of the pacman
///   struct Position
///   {
///       float x;   // pixel distance from left edge of window
///       float y;   // pixel distance from top edge of window
///   };
using Position = ge211::Posn<float>;

///   Direction the pacman is facing in
using Direction = ge211::Dims<float>;

struct Player {
    //
    // CONSTRUCTOR
    //

    //Player(Game_config const&);
    Player(int, Position, double, ge211::geometry::Dims<int>);

    /// Returns the position of the top-left corner of the pacmans
    /// "bounding box", meaning the smallest rectangle in which is can
    /// be enclosed. This is useful to the UI because sprites are
    /// positioned based on their top-left corners.
    Position top_left() const;

    /// Next state of Pacman after `dt` seconds
    Player next(double dt) const;

    /// Check if pacman can change direction without hitting a wall
    bool can_change_direction(Maze const&, double dt) const;

    /// Change the direction to be the next direction and next direction as {0,0}
    void change_direction();

    // These functions all perform collision detection

    /// Determines whether Pacman extends past a wall in the maze
    bool hits_wall(Maze const&) const;

    /// If pacman is very close to the wall border while moving, then stick it to the wall border
    void stick_to_wall(Maze const&,float dt);

    /// If Pacman touches a consumeable, he consumes it and returns the index on the maze of that consumeable,
    /// otherwise, it returns -1
    /// Pacman is said to touch a consumable if it's body touches the center of the cell that the consumeable is in.
    int get_touched_dot(Maze const&) const;
    int get_touched_pellet(Maze const&) const;

    /// Determines whether Pacman's bounding box intersects with the
    /// given block.
    ///
    /// Intersection between a circle and a rectangle is tricky, so we
    /// will approximate it with the intersection of two rectangles.
    // bool hits_ghost(Ghosts) const;

    //
    // MEMBER VARIABLES
    //

    /// The radius of pacman.
    int radius;

    /// The position of pacman on the maze.
    Position position;

    /// The velocity of pacman in blocks per tick.
    double velocity;

    // The direction the pacman is facing in
    Direction direction;

    // The direction the pacman will face to when `direction` becomes {0,0}
    Direction next_direction;

    // About to make a turn
    bool turning;
};

/// Compares two `Player`s for equality. This may be useful for testing.
///
/// Two `Player`s are equal if all their member variables are pairwise
/// equal.
bool
operator==(Player const&, Player const&);

/// Inequality for `Player`s.
bool
operator!=(Player const&, Player const&);

/// Stream insertion (printing) for `Player`. This can make your test
/// results easier to read.
std::ostream&
operator<<(std::ostream&, Player const&);