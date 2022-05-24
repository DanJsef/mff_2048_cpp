#pragma once
#include "states.hpp"
#include "types.hpp"
#include <cstddef>
#include <ncurses.h>

class State;

class Control {
public:
  Control() { current_state = &MainMenu::get_instance(); }

  inline State *get_current_state() const { return current_state; }
  inline int get_input() const { return input; }

  inline void set_target(int t) { target = t; }
  inline int get_target() const { return target; }

  inline void set_size(int s) { size = s; }
  inline int get_size() const { return size; }

  inline void set_controls(Keyboard c) { controls = c; }
  inline Keyboard get_controls() const { return controls; }

  inline void set_mode(Mode m) { mode = m; }
  inline Mode get_mode() const { return mode; }

  inline void end() { running = false; }

  void render() {
    clear();
    current_state->render(this);
  }

  void toggle() {
    current_state->toggle(this);
    current_state->enter(this);
  }

  void user_input() {
    input = getch();
    switch (input) {
    case 'q':
      running = false;
      break;
    default:
      current_state->user_input(this);
    }
  }

  void set_state(State &newState) { current_state = &newState; }

  void game_loop() {
    while (running) {
      render();
      user_input();
    }
  }

private:
  State *current_state;
  bool running = true;
  int target = 2048;
  int size = 4;
  Mode mode = Mode::Player;
  Keyboard controls = Keyboard::Wasd;
  int input;
};
