#include "cmath"
#include "engine.hpp"
#include <ncurses.h>

/**
 * Find score for left move.
 *
 * @param board Current board state.
 * @return score for left move.
 */
inline int find_score_left(const board_type &board) {
  int score = 0;

  for (std::size_t row = 0; row < board.size(); ++row) {
    for (std::size_t col = 1; col < board.size(); ++col) {
      if (board[row][col] != 0) {
        int y = col - 1;

        // Edge case for most left position.
        if (y == 0 && board[row][y] == 0)
          ++score;

        while (y > 0 && board[row][y] == 0) {
          --y;
          ++score;
        }

        if (board[row][col] == board[row][y]) {
          score += 2 * std::log2(board[row][col]);
        }
      }
    }
  }

  return score;
}

/**
 * Find score for up move.
 *
 * @param board Current board state.
 * @return score for up move.
 */
inline int find_score_up(const board_type &board) {
  int score = 0;

  for (std::size_t row = 1; row < board.size(); ++row) {
    for (std::size_t col = 0; col < board.size(); ++col) {
      if (board[row][col] != 0) {
        int x = row - 1;

        // Edge case for most up position.
        if (x == 0 && board[x][col] == 0)
          ++score;

        while (x > 0 && board[x][col] == 0) {
          --x;
          ++score;
        }

        if (board[row][col] == board[x][col]) {
          score += 2 * std::log2(board[row][col]);
        }
      }
    }
  }

  return score;
}

/**
 * Find score for right move.
 *
 * @param board Current board state.
 * @return score for right move.
 */
inline int find_score_right(const board_type &board) {
  int score = 0;

  for (std::size_t row = 0; row < board.size(); ++row) {
    for (std::size_t col = 0; col < board.size() - 1; ++col) {
      if (board[row][col] != 0) {
        int y = col + 1;

        // Edge case for most right position.
        if (y == (int)board.size() - 1 && board[row][y] == 0)
          ++score;

        while (y < (int)board.size() - 1 && board[row][y] == 0) {
          ++y;
          ++score;
        }

        if (board[row][col] == board[row][y]) {
          score += 2 * std::log2(board[row][col]);
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
