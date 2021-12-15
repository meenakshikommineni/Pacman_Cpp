
#include "space.hxx"

std::ostream&
operator<<(std::ostream& os, Space s)
{
    switch (s) {
    case Space::wall:
        return os << "W";
    case Space::dot:
        return os << ".";
    case Space::pellet:
        return os << "0";
    default:
        return os << "_";
    }
}

