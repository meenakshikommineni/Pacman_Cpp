#pragma once

#include "model.hxx"
#include "game_config.hxx"

static int const FONT_SIZE = 15;

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

private:
    Model const& model_;

    // for blinking ghost
    int blink_n_ = 0;

    // sprites to render on the screen
    ge211::Circle_sprite const pacman_body_sprite_;
    ge211::Circle_sprite const normal_ghost_body_sprites_[4];
    ge211::Circle_sprite const blue_ghost_body_sprite_;
    ge211::Circle_sprite const white_ghost_body_sprite_;
    ge211::Rectangle_sprite const wall_sprite_;
    ge211::Circle_sprite const dot_sprite_;
    ge211::Circle_sprite const pellet_sprite_;
     
    ge211::Font const default_font{"PressStart2P-Regular.ttf", FONT_SIZE};

    ge211::Text_sprite const high_score_label{"HIGH SCORE", default_font};
    ge211::Text_sprite const score_label{"SCORE", default_font};
    ge211::Text_sprite const lives_label{"LIVES", default_font};
    ge211::Text_sprite high_score_sprite;
    ge211::Text_sprite score_sprite;

    ge211::Text_sprite status_sprite;
};
