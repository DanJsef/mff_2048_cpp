#pragma once
#include <algorithm>
#include <random>
#include <tuple>
#include <vector>

using board_type = std::vector<std::vector<int>>;

class Engine {
public:
  /**
   * Setup initial state.
   *
   * @param size Size of game board.
   * @param target Target needed for win.
   */
  void setup(int size, int target) {
    board = board_type(size, std::vector<int>(size, 0));
    tile_count = 0;
    moved = false;
    this->target = target;
    highest = 4;
    new_tile();
    new_tile();
  }

  /**
   * Move tiles to left.
   *
   */
  void left() {
    compress_left(false);
    merge_left(false);
    compress_left(false);
    add_tile();
  }

  /**
   * Move tiles to right.
   *
   */
  void right() {
    compress_left(true);
    merge_left(true);
    compress_left(true);
    add_tile();
  }

  /**
   * Move tiles up.
   *
   */
  void up() {
    compress_up(false);
    merge_up(false);
    compress_up(false);
    add_tile();
  }

  /**
   * Move tiles down.
   *
   */
  void down() {
    compress_up(true);
    merge_up(true);
    compress_up(true);
    add_tile();
  }

  /**
   * Get current game board state.
   *
   * @return current board state.
   */
  const board_type &get_board() const { return board; }

  /**
   * Check if player won the game.
   *
   * @return true if target was reached.
   */
  bool check_win() const { return (target == highest); }

  /*
   * Check if player lost the game.
   *
   * @return true if no tile can be moved.
   */
  bool check_lose() const {

    static const std::vector<std::tuple<int, int>> neighbours_offset = {
        std::tuple<int, int>(0, 1), std::tuple<int, int>(1, 0),
        std::tuple<int, int>(0, -1), std::tuple<int, int>(-1, 0)};

    if (tile_count == (int)(board.size() * board.size())) {

      for (std::size_t row = 0; row < board.size(); ++row)
        for (std::size_t col = 0; col < board.size(); ++col) {

          for (const auto &neighbour_offset : neighbours_offset) {
            int x = row + std::get<0>(neighbour_offset);
            int y = col + std::get<1>(neighbour_offset);

            if (x >= 0 && y >= 0 && x < (int)board.size() &&
                y < (int)board.size() && board[x][y] == board[row][col])
              return false;
          }
        }
      return true;
    }

    return false;
  }

private:
  board_type board;
  int tile_count;
  int highest;
  int target;
  bool moved;

  /**
   * Generate random value from 0 to specified number.
   *
   * @param to Upper bound of sample range.
   * @return random value.
   */
  int random(int to) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, to);
    return dist(gen);
  }

  /**
   * Generate new tile to random position on game board.
   *
   * Newly added tile has value of 2 or 4 (2 with probability 0.7)
   *
   */
  void new_tile() {
    std::vector<std::tuple<int, int>> free;

    // find all free positions on game board
    for (std::size_t x = 0; x < board.size(); ++x)
      for (std::size_t y = 0; y < board.size(); ++y)
        if (board[x][y] == 0)
          free.push_back(std::make_tuple(x, y));

    // select random free position
    int position = random(free.size() - 1);

    // Choose tile value
    int value = 2;
    if (random(10) > 7)
      value = 4;

    board[std::get<0>(free[position])][std::get<1>(free[position])] = value;
    ++tile_count;
  }

  /**
   * Add new tile if game board state change in the last move.
   *
   */
  void add_tile() {
    if (moved) {
      new_tile();
      moved = false;
    }
  }

  /**
   * Compress tiles to left or right.
   *
   * Move all tiles to the chosen direction until they colide with another tile.
   *
   * @param reversed If true tiles are compressed to right.
   */
  void compress_left(bool reversed) {
    for (std::size_t row = 0; row < board.size(); ++row) {

      if (reversed)
        std::reverse(board[row].begin(), board[row].end());

      for (std::size_t col = 0; col < board.size(); ++col) {

        if (board[row][col] != 0) {
          int x = row;
          int y = col;

          while (y > 0 && board[x][y - 1] == 0) {
            std::swap(board[x][y], board[x][y - 1]);
            moved = true;
            --y;
          }
        }
      }

      if (reversed)
        std::reverse(board[row].begin(), board[row].end());
    }
  }

  /**
   * Compress tiles up or down.
   *
   * Move all tiles to the chosen direction until they colide with another tile.
   *
   * @param reversed If true tiles are compressed down.
   */
  void compress_up(bool reversed) {
    if (reversed)
      std::reverse(board.begin(), board.end());

    for (std::size_t row = 0; row < board.size(); ++row) {
      for (std::size_t col = 0; col < board.size(); ++col) {

        if (board[row][col] != 0) {
          int x = row;
          int y = col;

          while (x > 0 && board[x - 1][y] == 0) {
            std::swap(board[x][y], board[x - 1][y]);
            moved = true;
            --x;
          }
        }
      }
    }

    if (reversed)
      std::reverse(board.begin(), board.end());
  }

  /**
   * Merge adjacent tiles to left or right.
   *
   * @param reversed If true tiles are merged to right.
   */
  void merge_left(bool reversed) {
    for (std::size_t row = 0; row < board.size(); ++row) {

      if (reversed)
        std::reverse(board[row].begin(), board[row].end());

      for (std::size_t col = 1; col < board.size(); ++col) {

        if (board[row][col] != 0 && board[row][col - 1] == board[row][col]) {

          board[row][col - 1] = board[row][col - 1] * 2;
          board[row][col] = 0;
          --tile_count;
          moved = true;

          if (board[row][col - 1] > highest)
            highest = board[row][col - 1];
        }
      }

      if (reversed)
        std::reverse(board[row].begin(), board[row].end());
    }
  }

  /**
   * Merge adjacent tiles up or down.
   *
   * @param reversed If true tiles are merged down.
   */
  void merge_up(bool reversed) {
    if (reversed)
      std::reverse(board.begin(), board.end());

    for (std::size_t row = 1; row < board.size(); ++row) {
      for (std::size_t col = 0; col < board.size(); ++col) {

        if (board[row][col] != 0 && board[row - 1][col] == board[row][col]) {
          board[row - 1][col] = board[row - 1][col] * 2;
          board[row][col] = 0;
          --tile_count;
          moved = true;

          if (board[row - 1][col] > highest)
            highest = board[row - 1][col];
        }
      }
    }

    if (reversed)
      std::reverse(board.begin(), board.end());
  }
};
