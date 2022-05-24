#include "cmath"
#include "engine.hpp"
#include <ncurses.h>

inline int find_score_left(const board_type &board) {
  int score = 0;

  for (std::size_t r = 0; r < board.size(); ++r) {
    for (std::size_t c = 1; c < board.size(); ++c) {
      if (board[r][c] != 0) {
        int y = c - 1;
        if (y == 0 && board[r][y] == 0)
          ++score;
        while (y > 0 && board[r][y] == 0) {
          --y;
          ++score;
        }
        if (board[r][c] == board[r][y]) {
          score += 2 * std::log2(board[r][c]);
        }
      }
    }
  }

  return score;
}

inline int find_score_up(const board_type &board) {
  int score = 0;

  for (std::size_t r = 1; r < board.size(); ++r) {
    for (std::size_t c = 0; c < board.size(); ++c) {
      if (board[r][c] != 0) {
        int x = r - 1;
        if (x == 0 && board[x][c] == 0)
          ++score;
        while (x > 0 && board[x][c] == 0) {
          --x;
          ++score;
        }
        if (board[r][c] == board[x][c]) {
          score += 2 * std::log2(board[r][c]);
        }
      }
    }
  }

  return score;
}

inline int find_score_right(const board_type &board) {
  int score = 0;

  for (std::size_t r = 0; r < board.size(); ++r) {
    for (std::size_t c = 0; c < board.size() - 1; ++c) {
      if (board[r][c] != 0) {
        int y = c + 1;
        if (y == (int)board.size() - 1 && board[r][y] == 0)
          ++score;
        while (y < (int)board.size() - 1 && board[r][y] == 0) {
          ++y;
          ++score;
        }
        if (board[r][c] == board[r][y]) {
          score += 2 * std::log2(board[r][c]);
        }
      }
    }
  }

  return score;
}

inline void solve(Engine &engine) {
  int score_left = find_score_left(engine.get_board());
  int score_up = find_score_up(engine.get_board());
  int score_right = find_score_right(engine.get_board());

  if (score_left > 0 && score_left >= score_up)
    engine.left();
  else if (score_up > 0)
    engine.up();
  else if (score_right > 0)
    engine.right();
  else
    engine.down();
}
