#pragma once

#include "space.hxx"

#include <ge211.hxx>

const int MAZE_WIDTH = 19;
const int MAZE_HEIGHT = 21;

using Maze = std::array<Space, MAZE_WIDTH * MAZE_HEIGHT>;