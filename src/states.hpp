#pragma once
#include "control.hpp"
#include <cstddef>
#include <ncurses.h>
#include <string>
#include <vector>

class Control;

class State {
public:
  virtual void enter(Control *control) = 0;
  virtual void toggle(Control *control) = 0;
  virtual void exit(Control *control) = 0;
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
  void enter(Control *control) override {}
  void toggle(Control *control) override;
  void exit(Control *control) override {}

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
  void enter(Control *control) override {}
  void toggle(Control *control) override;
  void exit(Control *control) override {}

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
  void enter(Control *control) override {}
  void toggle(Control *control) override;
  void exit(Control *control) override {}

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

class Game : State {
public:
  void enter(Control *control) {}
  void toggle(Control *control);
  void userInput(Control *control);
  void exit(Control *control) {}
  void render(Control *control) const;
  static State &getInstance() {
    static Game singleton;
    return singleton;
  }

private:
  bool finished = false;
  int count = 0;
};
