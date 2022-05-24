#pragma once
#include "control.hpp"
#include "engine.hpp"
#include <cstddef>
#include <ncurses.h>
#include <string>
#include <vector>

class Control;

class State {
public:
  virtual void toggle(Control *control) = 0;
  virtual void enter(Control *control) = 0;
  virtual void render(Control *control) const = 0;
  virtual void userInput(Control *control) = 0;
};

class Menu : public State {
public:
  void userInput(Control *control) override;
  void render(Control *control) const override;

protected:
  std::string name;
  std::vector<std::string> choices;
  std::size_t choice = 0;
};

class MainMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override {}

  static State &getInstance() {
    static MainMenu singleton;
    return singleton;
  }

private:
  MainMenu() {
    name = "Main menu";
    choices = {"Start", "Target", "Size", "Quit"};
  }
};

class TargetMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override {}

  static State &getInstance() {
    static TargetMenu singleton;
    return singleton;
  }

private:
  TargetMenu() {
    name = "Target menu";
    choices = {"128", "256", "512", "1024", "2048"};
  }
};

class SizeMenu : Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override {}

  static State &getInstance() {
    static SizeMenu singleton;
    return singleton;
  }

private:
  SizeMenu() {
    name = "Size menu";
    choices = {"4", "5", "6"};
  }
};

class EndMenu : public Menu {
public:
  void toggle(Control *control) override;
  void enter(Control *control) override {}

protected:
  EndMenu() { choices = {"Restart", "Main menu", "Quit"}; }
};

class WinMenu : public EndMenu {
public:
  static State &getInstance() {
    static WinMenu singleton;
    return singleton;
  }

private:
  WinMenu() { name = "Congrats, you won!"; }
};

class LoseMenu : public EndMenu {
public:
  static State &getInstance() {
    static LoseMenu singleton;
    return singleton;
  }

private:
  LoseMenu() { name = "Game Over"; }
};

class Game : State {
public:
  void toggle(Control *control) override;
  void userInput(Control *control) override;
  void render(Control *control) const override;
  void enter(Control *control) override;

  static State &getInstance() {
    static Game singleton;
    return singleton;
  }

private:
  void render_tile(int x, int y, int value) const;
  std::string create_tile(std::string s, size_t length) const;
  Engine engine;
};
