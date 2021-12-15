
#include "game_config.hxx"


std::string
init_maze_str() {
    return
"\
wwwwwwwwwwwwwwwwwww\
w........w........w\
wpww.www.w.www.wwpw\
w.................w\
w.ww.w.wwwww.w.ww.w\
w....w...w...w....w\
wwww.www.w.www.wwww\
   w.w       w.w   \
wwww.w ww ww w.wwww\
    .  w   w  .    \
wwww.w wwwww w.wwww\
   w.w       w.w   \
wwww.w wwwww w.wwww\
w........w........w\
w.ww.www.w.www.ww.w\
wp.w...........w.pw\
ww.w.w.wwwww.w.w.ww\
w....w...w...w....w\
w.wwwwww.w.wwwwww.w\
w.................w\
wwwwwwwwwwwwwwwwwww";
}

// This is the default (and only) constructor for `Game_config`. It
// determines all the default values of all the member variables.
Game_config::Game_config()
        : scene_dims {1024, 768},
          block_size {22},
          pacman_radius {10},
          pacman_velocity_0 {2},
          pacman_position_0 {9,11},
          ghost_radius {10},
          ghost_velocity_0 {2},
          ghost_position_0 {{9,8},{8,9},{9,9},{10,9}},
          top_margin {100},
          side_margin {170},
          maze_str (init_maze_str()),
          high_score_file ("pacman_high_score.score")
{ }