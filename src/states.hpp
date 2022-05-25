#pragma once
#include "control.hpp"
#include "engine.hpp"
#include "types.hpp"
#include <cstddef>
#include <ncurses.h>
#include <string>
#include <vector>

class Control;

class State {
public:
  /**
   * Toggle to the next state.
   *
   * @param *control Pointer to the control which holds the state.
   */
  virtual void toggle(Control *control) = 0;

  /**
   * Initialize state.
   *
   * @param *control Pointer to the control which holds the state.
   */
  virtual void enter(Control *control) = 0;

  /**
   * Render state.
   *
   * @param *control Pointer to the control which holds the state.
   */
  virtual void render(Control *control) const = 0;

  /**
   * Handle user input in state.
   *
   * @param *control Pointer to the control which holds the state.
   */
  virtual void user_input(Control *control) = 0;
};

class Menu : public State {
public:
  void user_input(Control *control) override;
  void render(Control *control) const override;

protected:
  std::string name;
  std::vector<std::string> choices;
  int choice = 0;
};

class MainMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

  static State &get_instance() {
    static MainMenu singleton;
    return singleton;
  }

private:
  MainMenu() {
    name = "Main menu";
    choices = {"Start", "Target", "Size", "Mode", "Controls", "Quit"};
  }
};

class TargetMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

  static State &get_instance() {
    static TargetMenu singleton;
    return singleton;
  }

private:
  TargetMenu() {
    name = "Select target";
    choices = {"128", "256", "512", "1024", "2048"};
  }
};

class SizeMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

  static State &get_instance() {
    static SizeMenu singleton;
    return singleton;
  }

private:
  SizeMenu() {
    name = "Select board size";
    choices = {"4", "5", "6"};
  }
};

class ControlsMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

  static State &get_instance() {
    static ControlsMenu singleton;
    return singleton;
  }

private:
  ControlsMenu() {
    name = "Select controls";
    choices = {"WASD", "Arrows"};
  }
};

class ModeMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

  static State &get_instance() {
    static ModeMenu singleton;
    return singleton;
  }

private:
  ModeMenu() {
    name = "Select mode";
    choices = {"Player", "Solver"};
  }
};

class EndMenu : public Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override { (void)control; }

protected:
  EndMenu() { choices = {"Restart", "Main menu", "Quit"}; }
};

class WinMenu : public EndMenu {
public:
  static State &get_instance() {
    static WinMenu singleton;
    return singleton;
  }

private:
  WinMenu() { name = "Congrats, you won!"; }
};

class LoseMenu : public EndMenu {
public:
  static State &get_instance() {
    static LoseMenu singleton;
    return singleton;
  }

private:
  LoseMenu() { name = "Game Over"; }
};

class Game : State {
public:
  void toggle(Control *control) override;
  void user_input(Control *control) override;
  void render(Control *control) const override;
  void enter(Control *control) override;

  static State &get_instance() {
    static Game singleton;
    return singleton;
  }

private:
  Engine engine;

  /**
   * Render tile to selected position on screen.
   *
   * @param x Location on x axis.
   * @param y Location on y axis.
   * @param value Value of the tile.
   */
  void render_tile(int x, int y, int value) const;

  /**
   * Generate tile shape.
   *
   * @param value Value of the tile represented as string.
   */
  std::string create_tile(std::string value) const;

  /**
   * Handle input for player mode.
   */
  void player_input(std::vector<int> controls, int input);

  /**
   * Get vector of selected controls.
   *
   * @return std::vector with selected controls.
   */
  const std::vector<int> get_controls(Keyboard controls) const {
    switch (controls) {
    case Keyboard::Arrows:
      return std::vector<int>{KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN};
    case Keyboard::Wasd:
      return std::vector<int>{
          'a',
          'd',
          'w',
          's',
      };
    }
    __builtin_unreachable();
  }
};
