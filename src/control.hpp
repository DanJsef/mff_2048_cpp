#pragma once
#include "states.hpp"
#include "types.hpp"
#include <cstddef>
#include <ncurses.h>

class State;

class Control {
public:
  Control() { currentState = &MainMenu::getInstance(); }

  inline State *getCurrentState() const { return currentState; }
  inline int getInput() const { return input; }

  inline void setTarget(std::size_t t) { target = t; }
  inline std::size_t getTarget() const { return target; }

  inline void setSize(std::size_t s) { size = s; }
  inline std::size_t getSize() const { return size; }

  inline void setControls(Keyboard c) { controls = c; }
  inline Keyboard getControls() const { return controls; }

  inline void setMode(Mode m) { mode = m; }
  inline Mode getMode() const { return mode; }

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
  Keyboard controls = Keyboard::Wasd;
  int input;
};
