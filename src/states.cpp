#include "control.hpp"
#include "solver.hpp"
#include <cmath>
#include <ncurses.h>
#include <string>

class Control;

void Menu::user_input(Control *control) {
  switch (control->get_input()) {
  case 'w':
  case KEY_UP:
    if (choice > 0)
      --choice;
    break;
  case 's':
  case KEY_DOWN:
    if (choice < (int)choices.size() - 1)
      ++choice;
    break;
  case '\n':
    control->toggle();
    break;
  }
}

void Menu::render(Control *control) const {
  (void)control;
  printw("%s", name.c_str());
  for (int i = 0; i < (int)choices.size(); ++i) {
    if (i == choice)
      attron(A_REVERSE);
    mvprintw(i + 2, 0, "%s", choices[i].c_str());
    attroff(A_REVERSE);
  }
}

void MainMenu::toggle(Control *control) {
  if (choices[choice] == "Start")
    control->set_state(Game::get_instance());
  else if (choices[choice] == "Target")
    control->set_state(TargetMenu::get_instance());
  else if (choices[choice] == "Size")
    control->set_state(SizeMenu::get_instance());
  else if (choices[choice] == "Mode")
    control->set_state(ModeMenu::get_instance());
  else if (choices[choice] == "Controls")
    control->set_state(ControlsMenu::get_instance());
  else if (choices[choice] == "Quit")
    control->end();
}

void TargetMenu::toggle(Control *control) {
  control->set_target(std::stoi(choices[choice]));
  control->set_state(MainMenu::get_instance());
}

void SizeMenu::toggle(Control *control) {
  control->set_size(std::stoi(choices[choice]));
  control->set_state(MainMenu::get_instance());
}

void ControlsMenu::toggle(Control *control) {
  if (choices[choice] == "WASD")
    control->set_controls(Keyboard::Wasd);
  else if (choices[choice] == "Arrows")
    control->set_controls(Keyboard::Arrows);
  control->set_state(MainMenu::get_instance());
}

void ModeMenu::toggle(Control *control) {
  if (choices[choice] == "Player")
    control->set_mode(Mode::Player);
  else if (choices[choice] == "Solver")
    control->set_mode(Mode::Solver);
  control->set_state(MainMenu::get_instance());
}

void EndMenu::toggle(Control *control) {
  if (choices[choice] == "Restart")
    control->set_state(Game::get_instance());
  else if (choices[choice] == "Main menu")
    control->set_state(MainMenu::get_instance());
  else if (choices[choice] == "Quit")
    control->end();
}

void Game::toggle(Control *control) {
  if (engine.check_win())
    control->set_state(WinMenu::get_instance());
  else if (engine.check_lose())
    control->set_state(LoseMenu::get_instance());
  else
    control->set_state(MainMenu::get_instance());
}

void Game::player_input(std::vector<int> controls, int input) {
  if (controls[0] == input)
    engine.left();
  else if (controls[1] == input)
    engine.right();
  else if (controls[2] == input)
    engine.up();
  else if (controls[3] == input)
    engine.down();
}

void Game::user_input(Control *control) {
  if ('x' == control->get_input())
    control->toggle();
  else {
    switch (control->get_mode()) {
    case Mode::Player:
      player_input(get_controls(control->get_controls()), control->get_input());
      break;
    case Mode::Solver:
      if ('n' == control->get_input())
        solve(engine);
      break;
    }
  }
  if (engine.check_win() || engine.check_lose())
    control->toggle();
}

void Game::enter(Control *control) {
  engine.setup(control->get_size(), control->get_target());
}

std::string Game::create_tile(std::string s) const {
  const int padding_right = (6 - s.length()) / 2;
  const int padding_left = 6 - s.length() - padding_right;

  s.insert(0, std::string(padding_left, ' '));
  s.insert(s.length(), std::string(padding_right, ' '));

  return s;
}

void Game::render_tile(int x, int y, int value) const {
  std::string s = create_tile(std::to_string(value));
  y = y * 3;
  x = x * 6;

  attron(COLOR_PAIR(std::log2(value)));
  mvprintw(y, x, "      ");
  mvprintw(y + 1, x, "%s", s.c_str());
  mvprintw(y + 2, x, "      ");
  attroff(COLOR_PAIR(std::log2(value)));
}

void Game::render(Control *control) const {
  for (int x = 0; x < control->get_size(); ++x)
    for (int y = 0; y < control->get_size(); ++y)
      render_tile(y, x, engine.get_board()[x][y]);
}
