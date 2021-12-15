#include "view.hxx"
#include "space.hxx"

static ge211::Color const pacman_body_color {255, 255, 0};
static ge211::Color const normal_ghost_body_color_0 {255, 0,0};
static ge211::Color const normal_ghost_body_color_1 {255, 184, 255};
static ge211::Color const normal_ghost_body_color_2 {0, 255, 255};
static ge211::Color const normal_ghost_body_color_3 {255, 184, 82};
static ge211::Color const blue_ghost_body_color {25, 25, 166};
static ge211::Color const white_ghost_body_color {255, 255, 255};
static ge211::Color const wall_color {0,0,155};
static ge211::Color const dot_color {255,255,155};;
static ge211::Color const pellet_color {255,255,155};
static ge211::Color const text_color {255, 255, 255};

static int const grid_height = MAZE_HEIGHT;
static int const grid_width = MAZE_WIDTH;

View::View(Model const& model)
        : model_(model),
        pacman_body_sprite_(this->model_.pacman.radius, pacman_body_color),
        normal_ghost_body_sprites_{
            ge211::Circle_sprite(this->model_.ghosts[0].radius, normal_ghost_body_color_0),
            ge211::Circle_sprite(this->model_.ghosts[1].radius, normal_ghost_body_color_1),
            ge211::Circle_sprite(this->model_.ghosts[2].radius, normal_ghost_body_color_2),
            ge211::Circle_sprite(this->model_.ghosts[3].radius, normal_ghost_body_color_3),
        },
        blue_ghost_body_sprite_(this->model_.ghosts[0].radius, blue_ghost_body_color),
        white_ghost_body_sprite_(this->model_.ghosts[0].radius, white_ghost_body_color),
        wall_sprite_({5*this->model_.config.block_size/6,5*this->model_.config.block_size/6}, wall_color),
        dot_sprite_(this->model_.config.block_size/4, dot_color),
        pellet_sprite_(this->model_.config.block_size/2, pellet_color)
{ }

void
View::draw(ge211::Sprite_set& set)
{
    auto windims = initial_window_dimensions();
    auto maze_start_y = windims.height/2 - this->model_.config.block_size*grid_height/2;
    int maze_start_x = windims.width/2 - this->model_.config.block_size*grid_width/2;

    // Draw Pacman
    Position pacman_top_left = {
            maze_start_x + this->model_.pacman.position.x * this->model_.config.block_size,
            maze_start_y + this->model_.pacman.position.y * this->model_.config.block_size
        };
    set.add_sprite(this->pacman_body_sprite_, {static_cast<int>(pacman_top_left.x), static_cast<int>(pacman_top_left.y)}, 1);

    // Draw the maze
    ge211::Dims<int> dot_padding = {this->model_.config.block_size/4, this->model_.config.block_size/4};

    for(auto i=0;i<grid_height; i++) {
        for(auto j=0;j<grid_width; j++) {
            ge211::Posn<int> pos = {maze_start_x+j*this->model_.config.block_size,maze_start_y+ i*this->model_.config.block_size};
            switch (this->model_.maze[i*grid_width + j]) {
            case Space::wall:
                set.add_sprite(this->wall_sprite_, pos);
                break;
            case Space::dot:
                set.add_sprite(this->dot_sprite_, pos + dot_padding);
                break;
            case Space::pellet:
                set.add_sprite(this->pellet_sprite_, pos);
                break;
            default:
                break;
            }
        }
    }

    // Draw the Ghosts
    for(int i = 0 ; i< 4; i++) {
        Position ghost_top_left = {
            maze_start_x + this->model_.ghosts[i].position.x * this->model_.config.block_size,
            maze_start_y + this->model_.ghosts[i].position.y * this->model_.config.block_size
        };
        const ge211::Circle_sprite* ghost_sprite = nullptr;
        switch (this->model_.ghosts[i].mode) {
        case GhostMode::blue:
            ghost_sprite = &this->blue_ghost_body_sprite_;
            if (this->model_.blue_mode_counter < 4) {
                if((this->blink_n_/10)%2) {
                    ghost_sprite = &this->white_ghost_body_sprite_;
                }
                this->blink_n_ ++;
            }
            break;
        case GhostMode::white:
            ghost_sprite = &this->white_ghost_body_sprite_;
            break;
        default:
            ghost_sprite = &this->normal_ghost_body_sprites_[i];
            break;
        }
        set.add_sprite(*ghost_sprite, {static_cast<int>(ghost_top_left.x), static_cast<int>(ghost_top_left.y)}, 1);
    }

    // Draw the labels
    set.add_sprite(this->high_score_label, {10,10});
    auto bottom_label_y = this->model_.config.scene_dims.height - FONT_SIZE*3 - 10;
    auto score_label_x = this->model_.config.scene_dims.width - FONT_SIZE*5 - 10;
    set.add_sprite(this->score_label,
        {score_label_x, bottom_label_y});
    set.add_sprite(this->lives_label, {10, bottom_label_y});

    // Draw label values - score, high score, lives 
    ge211::Text_sprite::Builder score_builder(this->default_font);
    score_builder.color(text_color) << this->model_.score;
    this->score_sprite.reconfigure(score_builder);
    set.add_sprite(this->score_sprite, {score_label_x, bottom_label_y + FONT_SIZE + 10});
    ge211::Text_sprite::Builder high_score_builder(this->default_font);
    high_score_builder.color(text_color) << this->model_.high_score;
    this->high_score_sprite.reconfigure(high_score_builder);
    set.add_sprite(this->high_score_sprite, {10, FONT_SIZE + 20});

    for (int i =0;i<this->model_.lives;i++) {
        set.add_sprite(this->pacman_body_sprite_, {10 + i*(this->model_.pacman.radius*2+10), bottom_label_y + FONT_SIZE + 10});
    }

    // game status
    ge211::Text_sprite::Builder status_builder(this->default_font);
    switch (this->model_.state) {
    case GameState::ready:
        status_builder.color(text_color) << "PRESS ANY KEY TO START - ";
        break;
    case GameState::gameover:
        status_builder.color(text_color) << "GAME OVER. YOU MADE IT TO ";
        break;
    default:
        break;
    }
    status_builder.color(text_color) << "LEVEL " << this->model_.level;
    this->status_sprite.reconfigure(status_builder);
    set.add_sprite(this->status_sprite, {this->model_.config.scene_dims.width/2 - 50, 10});

}

View::Dimensions
View::initial_window_dimensions() const
{
    return {this->model_.config.scene_dims.width,this->model_.config.scene_dims.height};
}

std::string
View::initial_window_title() const
{
    return "Pacman";
}