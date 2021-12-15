#include "controller.hxx"

Controller::Controller()
        : view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& set)
{
    view_.draw(set);
}


void
Controller::on_frame(double dt)
{
    model_.on_frame(dt);
}


void
Controller::on_key(ge211::Key key)
{
    if(this->model_.state == GameState::ready) {
        this->model_.state = GameState::running;
    } else if(this->model_.state == GameState::gameover) {
        this->model_.restart();
    }
    if (key == ge211::Key::up()) {
        this->model_.go_up();
        return;
    } else if (key == ge211::Key::right()) {
        this->model_.go_right();
        return;
    } else if (key == ge211::Key::left()) {
        this->model_.go_left();
        return;
    } else if (key == ge211::Key::down()) {
        this->model_.go_down();
        return;
    } else if (key == ge211::Key::code('s')) { //see highscore & score on console
        std::cout << this->model_.high_score << ", " << this->model_.score << std::endl;
        return;
    } else if (key == ge211::Key::code('i')) { //increase score by 1
        this->model_.score++;
        return;
    }
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}
