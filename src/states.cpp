#include "control.hpp"
#include <cmath>
#include <ncurses.h>
#include <string>

class Control;

void Menu::userInput(Control *control) {
  switch (control->getInput()) {
  case 'w':
  case KEY_UP:
    if (choice > 0)
      --choice;
    break;
  case 's':
  case KEY_DOWN:
    if (choice < choices.size() - 1)
      ++choice;
    break;
  case '\n':
    control->toggle();
    break;
  }
}

void Menu::render(Control *control) const {
  printw("%s", name.c_str());
  for (std::size_t i = 0; i < choices.size(); ++i) {
    if (i == choice)
      attron(A_REVERSE);
    mvprintw(i + 2, 0, "%s", choices[i].c_str());
    attroff(A_REVERSE);
  }
}

void MainMenu::toggle(Control *control) {
  if (choices[choice] == "Start")
    control->setState(Game::getInstance());
  else if (choices[choice] == "Target")
    control->setState(TargetMenu::getInstance());
  else if (choices[choice] == "Size")
    control->setState(SizeMenu::getInstance());
  else if (choices[choice] == "Mode")
    control->setState(ModeMenu::getInstance());
  else if (choices[choice] == "Controls")
    control->setState(ControlsMenu::getInstance());
  else if (choices[choice] == "Quit")
    control->end();
}

void TargetMenu::toggle(Control *control) {
  control->setTarget(std::stoull(choices[choice]));
  control->setState(MainMenu::getInstance());
}

void SizeMenu::toggle(Control *control) {
  control->setSize(std::stoull(choices[choice]));
  control->setState(MainMenu::getInstance());
}

void ControlsMenu::toggle(Control *control) {
  if (choices[choice] == "WASD")
    control->setControls(Keyboard::Wasd);
  else if (choices[choice] == "Arrows")
    control->setControls(Keyboard::Arrows);
  control->setState(MainMenu::getInstance());
}

void ModeMenu::toggle(Control *control) {
  if (choices[choice] == "Player")
    control->setMode(Mode::Player);
  else if (choices[choice] == "Solver")
    control->setMode(Mode::Solver);
  control->setState(MainMenu::getInstance());
}

void EndMenu::toggle(Control *control) {
  if (choices[choice] == "Restart")
    control->setState(Game::getInstance());
  else if (choices[choice] == "Main menu")
    control->setState(MainMenu::getInstance());
  else if (choices[choice] == "Quit")
    control->end();
}

void Game::toggle(Control *control) {
  if (engine.check_win())
    control->setState(WinMenu::getInstance());
  else if (engine.check_lose())
    control->setState(LoseMenu::getInstance());
  else
    control->setState(MainMenu::getInstance());
}

void Game::userInput(Control *control) {
  const std::vector<int> controls = getControls(control->getControls());
  if (controls[0] == control->getInput())
    engine.left();
  else if (controls[1] == control->getInput())
    engine.right();
  else if (controls[2] == control->getInput())
    engine.up();
  else if (controls[3] == control->getInput())
    engine.down();
  else if ('x' == control->getInput())
    control->toggle();
  if (engine.check_win() || engine.check_lose())
    control->toggle();
}

void Game::enter(Control *control) {
  engine.setup(control->getSize(), control->getTarget());
}

std::string Game::create_tile(std::string s, size_t length) const {
  const int padding_right = (6 - s.length()) / 2;
  const int padding_left = 6 - s.length() - padding_right;

  s.insert(0, std::string(padding_left, ' '));
  s.insert(s.length(), std::string(padding_right, ' '));

  return s;
}

void Game::render_tile(int x, int y, int value) const {
  std::string s = create_tile(std::to_string(value), 6);
  y = y * 3;
  x = x * 6;

  attron(COLOR_PAIR(std::log2(value)));
  mvprintw(y, x, "      ");
  mvprintw(y + 1, x, "%s", s.c_str());
  mvprintw(y + 2, x, "      ");
  attroff(COLOR_PAIR(std::log2(value)));
}

void Game::render(Control *control) const {
  for (int x = 0; x < control->getSize(); ++x)
    for (int y = 0; y < control->getSize(); ++y)
      render_tile(y, x, engine.board[x][y]);
}
