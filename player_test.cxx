#include "player.hxx"
#include <catch.hxx>

TEST_CASE("Player moves correctly - move along a square")
{

    Position initial_pos = {0,0};
    Player player(4, initial_pos, 1, {1,0});

    // move 5 steps right
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);

    CHECK(player.position == Position{5,0});

    player.direction = {0,1};
    // move 5 steps down
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);

    CHECK(player.position == Position{5,5});

    player.direction = {-1,0};
    // move 5 steps left
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);

    CHECK(player.position == Position{0,5});

    player.direction = {0,-1};
    // move 5 steps up
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);
    player = player.next(1);

    // back to initial position
    CHECK(player.position == initial_pos);
}
