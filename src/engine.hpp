#pragma once
#include <algorithm>
#include <random>
#include <tuple>
#include <vector>

using boardType = std::vector<std::vector<int>>;

class Engine {
public:
  void setup(int size, std::size_t target) {
    board = boardType(size, std::vector<int>(size, 0));
    tile_count = 0;
    moved = false;
    this->target = target;
    highest = 4;
    new_tile();
    new_tile();
  }

  void left() {
    compress_left(false);
    merge_left(false);
    compress_left(false);
    add_tile();
  }
  void right() {
    compress_left(true);
    merge_left(true);
    compress_left(true);
    add_tile();
  }
  void up() {
    compress_up(false);
    merge_up(false);
    compress_up(false);
    add_tile();
  }
  void down() {
    compress_up(true);
    merge_up(true);
    compress_up(true);
    add_tile();
  }

  const boardType &getBoard() const { return board; }

  bool check_win() const { return (target == highest); }
  bool check_lose() const {

    static const std::vector<std::tuple<int, int>> neighbours = {
        std::tuple<int, int>(0, 1), std::tuple<int, int>(1, 0),
        std::tuple<int, int>(0, -1), std::tuple<int, int>(-1, 0)};

    if (tile_count == (board.size() * board.size())) {
      for (int r = 0; r < board.size(); ++r)
        for (int c = 0; c < board.size(); ++c) {
          for (const auto &neighbour : neighbours) {
            int x = r + std::get<0>(neighbour);
            int y = c + std::get<1>(neighbour);
            if (x >= 0 && y >= 0 && x < board.size() && y < board.size() &&
                board[x][y] == board[r][c])
              return false;
          }
        }
      return true;
    }
    return false;
  }

private:
  boardType board;
  int tile_count;
  int highest;
  int target;
  bool moved;

  int random(int to) const {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(0, to);
    return dist(gen);
  }
  void new_tile() {
    std::vector<std::tuple<int, int>> free;

    for (int x = 0; x < board.size(); ++x)
      for (int y = 0; y < board.size(); ++y)
        if (board[x][y] == 0)
          free.push_back(std::make_tuple(x, y));

    int position = random(free.size() - 1);

    int value = 2;

    if (random(10) > 7)
      value = 4;

    board[std::get<0>(free[position])][std::get<1>(free[position])] = value;
    ++tile_count;
  }

  void add_tile() {
    if (moved) {
      new_tile();
      moved = false;
    }
  }

  void compress_left(bool reversed) {
    for (int r = 0; r < board.size(); ++r) {
      if (reversed)
        std::reverse(board[r].begin(), board[r].end());
      for (int c = 0; c < board.size(); ++c) {
        if (board[r][c] != 0) {
          int x = r;
          int y = c;
          while (y > 0 && board[x][y - 1] == 0) {
            std::swap(board[x][y], board[x][y - 1]);
            moved = true;
            --y;
          }
        }
      }
      if (reversed)
        std::reverse(board[r].begin(), board[r].end());
    }
  }

  void compress_up(bool reversed) {
    if (reversed)
      std::reverse(board.begin(), board.end());
    for (int r = 0; r < board.size(); ++r) {
      for (int c = 0; c < board.size(); ++c) {
        if (board[r][c] != 0) {
          int x = r;
          int y = c;
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

  void merge_left(bool reversed) {
    for (int r = 0; r < board.size(); ++r) {
      if (reversed)
        std::reverse(board[r].begin(), board[r].end());
      for (int c = 1; c < board.size(); ++c) {
        if (board[r][c] != 0 && board[r][c - 1] == board[r][c]) {
          board[r][c - 1] = board[r][c - 1] * 2;
          board[r][c] = 0;
          --tile_count;
          moved = true;
          if (board[r][c - 1] > highest)
            highest = board[r][c - 1];
        }
      }
      if (reversed)
        std::reverse(board[r].begin(), board[r].end());
    }
  }

  void merge_up(bool reversed) {
    if (reversed)
      std::reverse(board.begin(), board.end());
    for (int r = 1; r < board.size(); ++r) {
      for (int c = 0; c < board.size(); ++c) {
        if (board[r][c] != 0 && board[r - 1][c] == board[r][c]) {
          board[r - 1][c] = board[r - 1][c] * 2;
          board[r][c] = 0;
          --tile_count;
          moved = true;
          if (board[r - 1][c] > highest)
            highest = board[r - 1][c];
        }
      }
    }
    if (reversed)
      std::reverse(board.begin(), board.end());
  }
};
