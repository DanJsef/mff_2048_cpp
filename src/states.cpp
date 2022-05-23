#include "control.hpp"
#include <ncurses.h>
#include <string>

class Control;

void Menu::userInput(Control *control) {
  switch (control->getInput()) {
  case 'w':
  case KEY_UP:
    if (choice == 0)
      choice = choices.size() - 1;
    else
      --choice;
    break;
  case 's':
  case KEY_DOWN:
    if (choice == choices.size() - 1)
      choice = 0;
    else
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

void Game::toggle(Control *control) {
  control->setState(MainMenu::getInstance());
}

void Game::userInput(Control *control) {
  switch (control->getInput()) {
  case 'd':
  case KEY_RIGHT:
    control->toggle();
    break;
  }
}

void Game::render(Control *control) const {
  printw("Size: %lu, Target: %lu", control->getSize(), control->getTarget());
}
