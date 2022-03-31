#ifndef INC_1C__CHECKERS_HPP_
#define INC_1C__CHECKERS_HPP_

#include <algorithm>
#include <iostream>
#include <vector>

enum class Mistakes {
  OK,
  NOT_OK
};


using std::vector;
using std::cout;
using std::cin;
using std::swap;
using std::find;

class GameField;
class Cell;
class Player;

enum class CellCondition { EMPTY = 0, WHITE_ON = 1, BLACK_ON = 2 };
enum class PlayerSide { WHITE = 1, BLACK = 2 };

struct Coordinates {
  int i;
  int j;
};

struct Moves {
  vector<Coordinates> to_eat;
  vector<Coordinates> to_move;
};

[[nodiscard]] bool InField(int i, int j);

//------------------------------------------------
//! Class Cell. Params and methods of cells
//------------------------------------------------
class Cell {
 public:
  Coordinates loc;
  CellCondition cond;
  bool king_on;
};

//------------------------------------------------
//! Class GameField. Params and methods of game
//! field
//------------------------------------------------
class GameField {
 public:
  GameField();
  ~GameField() = default;

  void ShowField();
  [[nodiscard]] Moves GetMoves(int loc_i, int loc_j);

  vector<vector<Cell>> field;
};

//------------------------------------------------
//! Class player. Params and methods of player
//------------------------------------------------
class Player {
 public:
  explicit Player(PlayerSide player_side);
  ~Player() = default;

  [[nodiscard]] Mistakes MakeMove(int prev_i,
                                  int prev_j,
                                  int i,
                                  int j,
                                  GameField& game_field) const;
  [[nodiscard]] Mistakes Eat(int prev_i,
                             int prev_j,
                             int i,
                             int j,
                             GameField& game_field) const;

  PlayerSide side;
};

#endif //INC_1C__CHECKERS_HPP_
