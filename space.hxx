
#pragma once

#include <iostream>

// Contents of the space on the maze
enum class Space
{
    dot,
    pellet,
    wall,
    empty,
};

// Prints a space in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Space);
