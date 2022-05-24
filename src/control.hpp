#pragma once
#include "states.hpp"
#include <cstddef>
#include <ncurses.h>

class State;

enum class Mode { Player, Solver };

class Control {
public:
  Control() { currentState = &MainMenu::getInstance(); }

  inline State *getCurrentState() const { return currentState; }
  inline int getInput() const { return input; }
  inline void setTarget(std::size_t t) { target = t; }
  inline std::size_t getTarget() const { return target; }
  inline void setSize(std::size_t s) { size = s; }
  inline std::size_t getSize() const { return size; }
  inline void end() { running = false; }

  void render() {
    clear();
    currentState->render(this);
  }

  void toggle() {
    currentState->toggle(this);
    currentState->enter(this);
  }

  void userInput() {
    input = getch();
    switch (input) {
    case 'q':
      running = false;
      break;
    default:
      currentState->userInput(this);
    }
  }

  void setState(State &newState) { currentState = &newState; }

  void game_loop() {
    while (running) {
      render();
      userInput();
    }
  }

private:
  State *currentState;
  bool running = true;
  std::size_t target = 2048;
  std::size_t size = 4;
  Mode mode = Mode::Player;
  int input;
};
