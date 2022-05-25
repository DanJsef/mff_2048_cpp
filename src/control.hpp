#pragma once
#include "states.hpp"
#include "types.hpp"
#include <cstddef>
#include <ncurses.h>

class State;

class Control {
public:
  Control() { current_state = &MainMenu::get_instance(); }

  // setters and getters
  inline State *get_current_state() const { return current_state; }
  inline int get_input() const { return input; }
  inline void set_state(State &newState) { current_state = &newState; }

  inline void set_target(int target) { this->target = target; }
  inline int get_target() const { return target; }

  inline void set_size(int size) { this->size = size; }
  inline int get_size() const { return size; }

  inline void set_controls(Keyboard controls) { this->controls = controls; }
  inline Keyboard get_controls() const { return controls; }

  inline void set_mode(Mode mode) { this->mode = mode; }
  inline Mode get_mode() const { return mode; }

  /**
   * Turns off the application.
   *
   */
  inline void end() { running = false; }

  /**
   * Render current state to screen.
   *
   */
  void render() {
    clear();
    current_state->render(this);
  }

  /**
   * Toggle to the next state and initialize the new state.
   *
   */
  void toggle() {
    current_state->toggle(this);
    current_state->enter(this);
  }

  /**
   * Read user input and process it or forward it to the current state.
   *
   */
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

  /**
   * Run the main application loop.
   *
   */
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
