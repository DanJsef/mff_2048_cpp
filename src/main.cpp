#include "control.hpp"
#include <ncurses.h>

void initialize_ncurses() {
  initscr();
  noecho();
  curs_set(0);
  start_color();
  keypad(stdscr, true);

  init_pair(0, COLOR_BLACK, COLOR_WHITE);
  init_pair(1, COLOR_BLACK, COLOR_CYAN);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(4, COLOR_BLACK, COLOR_BLUE);
  init_pair(5, COLOR_BLACK, COLOR_GREEN);
  init_pair(6, COLOR_BLACK, COLOR_RED);
  init_pair(7, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(8, COLOR_BLACK, COLOR_BLUE);
  init_pair(9, COLOR_BLACK, COLOR_GREEN);
  init_pair(10, COLOR_BLACK, COLOR_RED);
  init_pair(11, COLOR_BLACK, COLOR_YELLOW);
}

void end_ncurses() { endwin(); }

int main() {

  initialize_ncurses();

  Control control;

  control.game_loop();

  end_ncurses();
}
