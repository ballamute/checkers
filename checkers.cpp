#include "checkers.hpp"


/*--------------------------------GameField-----------------------------------*/
//------------------------------------------------
//! GameField class non-argument constructor
//!
//! @note Build default checkers field with black
//! and white checkers as 2 and 1 respectively
//------------------------------------------------
GameField::GameField() {
  field.resize(8);
  for (int i = 0; i < 8; ++i) {
    field[i].resize(8);
    for (int j = 0; j < 8; ++j) {
      field[i][j].loc = {i, j};
      field[i][j].king_on = false;
      if (i % 2 == 0 && j % 2 == 1 || i % 2 == 1 && j % 2 == 0) {
        if (i < 3) {
          field[i][j].cond = CellCondition::BLACK_ON;
        } else if (i > 4) {
          field[i][j].cond = CellCondition::WHITE_ON;
        }
      } else {
        field[i][j].cond = CellCondition::EMPTY;
      }
    }
  }
}

//------------------------------------------------
//! Shows us current positions on the game field
//!
//! @note number 1 is white (first player)
//! number 2 is black (second player)
//------------------------------------------------
void GameField::ShowField() {
  cout << "|||0 1 2 3 4 5 6 7\n";
  cout << "||||||||||||||||||\n";
  for (int i = 0; i < 8; ++i) {
    cout << i << "||";
    for (int j = 0; j < 8; ++j) {
      cout << (int) field[i][j].cond
           << ' '; // 0 - empty, 1 - white, 2 - black
    }
    cout << '\n';
  }
  cout << "-------------------------------" << '\n';
}


//------------------------------------------------
//! Gives us a pair of vectors: first - vector
//! with all available moves for checker, second
//! - with all available moves that will lead to
//! eating opponent`s checker
//! @param [in] loc_i checker vertical location
//! @param [in] loc_j checker horizontal location
//!
//! @return Moves -
//! - pair<vector<Coordinates>, vector<Coordinates>>
//!
//! @note In case of empty cell in params returns
//! empty vectors.
//------------------------------------------------
[[nodiscard]] Moves GameField::GetMoves(int loc_i, int loc_j) {
  Moves moves;
  Cell cell = field[loc_i][loc_j];
  CellCondition enemy_sell_cond;
  int sign;
  if (cell.cond == CellCondition::WHITE_ON) {
    enemy_sell_cond = CellCondition::BLACK_ON;
    sign = -1;
  } else if (cell.cond == CellCondition::BLACK_ON) {
    enemy_sell_cond = CellCondition::WHITE_ON;
    sign = 1;
  } else {
    return {vector<Coordinates>(0), vector<Coordinates>(0)};
  }

  if (cell.king_on) {
    std::pair<int, int> dir[4] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto& c: dir) {
      int k = 1;
      while (InField(cell.loc.i + c.first * k, cell.loc.j + c.second * k)) {
        if (field[cell.loc.i + c.first * k][cell.loc.j + c.second * k].cond
            == CellCondition::EMPTY) {
          moves.to_move.push_back({cell.loc.i + c.first * k,
                                   cell.loc.j + c.second * k});
        }
        if (field[cell.loc.i + c.first * k][cell.loc.j + c.second * k].cond
            == enemy_sell_cond) {
          ++k;
          while (
              field[cell.loc.i + c.first * k][cell.loc.j + c.second * k].cond
                  == CellCondition::EMPTY) {
            moves.to_eat.push_back({cell.loc.i + c.first * k,
                                    cell.loc.j + c.second * k});
            ++k;
          }
          break;
        }
        ++k;
      }
    }
  }

  if (InField(cell.loc.i + sign, cell.loc.j - 1)) {
    if (field[cell.loc.i + sign][cell.loc.j - 1].cond
        == CellCondition::EMPTY) {
      moves.to_move.push_back({cell.loc.i + sign, cell.loc.j - 1});
    } else if (
        field[cell.loc.i + sign][cell.loc.j - 1].cond == enemy_sell_cond &&
            field[cell.loc.i + 2 * sign][cell.loc.j - 2].cond
                == CellCondition::EMPTY) {
      moves.to_eat.push_back({cell.loc.i + 2 * sign, cell.loc.j - 2});
    }
  }
  if (InField(cell.loc.i + sign, cell.loc.j + 1)) {
    if (field[cell.loc.i + sign][cell.loc.j + 1].cond
        == CellCondition::EMPTY) {
      moves.to_move.push_back({cell.loc.i + sign, cell.loc.j + 1});
    } else if (
        field[cell.loc.i + sign][cell.loc.j + 1].cond == enemy_sell_cond &&
            field[cell.loc.i + 2 * sign][cell.loc.j + 2].cond
                == CellCondition::EMPTY) {
      moves.to_eat.push_back({cell.loc.i + 2 * sign, cell.loc.j + 2});
    }
  }
  return moves;
}

/*-----------------------------------Player-----------------------------------*/

//------------------------------------------------
//! Player class constructor
//! @param [in] player_side side of player

//! @note White or Black
//------------------------------------------------
Player::Player(PlayerSide player_side) {
  side = player_side;
}


//------------------------------------------------
//! Allows player to make an appropriate move
//! @param [in] prev_i from i coordinate
//! @param [in] prev_j from j coordinate
//! @param [in] i to i coordinate
//! @param [in] j to j coordinate
//! @param [out] game_field Game Field to play on
//!
//! @return Mistakes of writing params
//!
//! @note i - vertical, j - horizontal
//------------------------------------------------
[[nodiscard]] Mistakes Player::MakeMove(int prev_i,
                          int prev_j,
                          int i,
                          int j,
                          GameField& game_field) const {
  if (!InField(prev_i, prev_j)) {
    cout << "Wrong previous point coordinates\n";
    return Mistakes::NOT_OK;
  } else if (!InField(i, j)) {
    cout << "Wrong coordinates for point you want to move to\n";
    return Mistakes::NOT_OK;
  }
  if (game_field.field[prev_i][prev_j].cond == CellCondition::EMPTY) {
    cout << "You can`t move the emptiness\n";
    return Mistakes::NOT_OK;
  } else if ((int) game_field.field[prev_i][prev_j].cond != (int) side) {
    cout << "You can`t touch your opponent`s checker\n";
    return Mistakes::NOT_OK;
  }
  for (int it_1 = 0; it_1 < 8; it_1++) {
    for (int it_2 = 0; it_2 < 8; it_2++) {
      if (!game_field.GetMoves(it_1, it_2).to_eat.empty() &&
          game_field.field[prev_j][prev_j].cond
              == game_field.field[it_1][it_2].cond) {
        cout << "You have to eat now!\n";
        return Mistakes::NOT_OK;
      }
    }
  }
  vector<Coordinates> to_move = game_field.GetMoves(prev_i, prev_j).to_move;
  vector<Coordinates> to_eat = game_field.GetMoves(prev_i, prev_j).to_eat;

  bool is_moving = false;
  for (auto& it: to_move) {
    if (it.i == i && it.j == j) {
      is_moving = true;
    }
  }
  if (!is_moving) {
    cout << "Move is not allowed by the rules. "
            "The amount of the cells you can move on is "
         << to_move.size() << '\n';
    for (auto& el: to_move) {
      cout << el.i << ' ' << el.j << '\n';
    }
    return Mistakes::NOT_OK;
  }

  game_field.field[i][j].cond = game_field.field[prev_i][prev_j].cond;
  game_field.field[i][j].king_on = game_field.field[prev_i][prev_j].king_on;
  if (i == 0 || i == 7) {
    game_field.field[i][j].king_on = true;
  }
  game_field.field[prev_i][prev_j].cond = CellCondition::EMPTY;
  game_field.field[prev_i][prev_j].king_on = false;
  return Mistakes::OK;
}


//------------------------------------------------
//! Allows player to make an appropriate
//! eating move
//! @param [in] prev_i from i coordinate
//! @param [in] prev_j from j coordinate
//! @param [in] i to i coordinate
//! @param [in] j to j coordinate
//! @param [out] game_field Game Field to play on
//!
//! @return Mistakes of writing params
//!
//! @note i - vertical, j - horizontal
//------------------------------------------------
[[nodiscard]] Mistakes Player::Eat(int prev_i,
                     int prev_j,
                     int i,
                     int j,
                     GameField& game_field) const {
  if (!InField(prev_i, prev_j)) {
    cout << "Wrong previous point coordinates\n";
    return Mistakes::NOT_OK;
  } else if (!InField(i, j)) {
    cout << "Wrong coordinates for point you want to move to\n";
    return Mistakes::NOT_OK;
  }
  if (game_field.field[prev_i][prev_j].cond == CellCondition::EMPTY) {
    cout << "You can`t move the emptiness\n";
    return Mistakes::NOT_OK;
  } else if ((int) game_field.field[prev_i][prev_j].cond != (int) side) {
    cout << "You can`t touch your opponent`s checker\n";
    return Mistakes::NOT_OK;
  }
  vector<Coordinates> to_eat = game_field.GetMoves(prev_i, prev_j).to_eat;
  bool is_eating = false;
  for (auto& it: to_eat) {
    if (it.i == i && it.j == j) {
      is_eating = true;
    }
  }
  if (!is_eating) {
    cout << "This move cannot be called eating. "
            "The amount of the cells where you can it by going on them is "
         << to_eat.size() << '\n';
    for (auto& el: to_eat) {
      cout << el.i << ' ' << el.j << '\n';
    }
    return Mistakes::NOT_OK;
  }

  game_field.field[i][j].cond = game_field.field[prev_i][prev_j].cond;
  game_field.field[i][j].king_on = game_field.field[prev_i][prev_j].king_on;

  int i_iter = i < prev_i ? -1 : 1;
  int j_iter = j < prev_j ? -1 : 1;

  while (i != prev_i && j != prev_j) {
    game_field.field[prev_i][prev_j].cond = CellCondition::EMPTY;
    game_field.field[prev_i][prev_j].king_on = false;
    prev_i += i_iter;
    prev_j += j_iter;
  }
  return Mistakes::OK;
}

/*----------------------------------Other-------------------------------------*/

//------------------------------------------------
//! Checks if the coordinates are inside of
//! the game field
//! @param [in] i to i coordinate
//! @param [in] j to j coordinate
//!
//! @return bool
//!
//! @note borders i: [0, 7], j: [0, 7]
//------------------------------------------------
[[nodiscard]] bool InField(int i, int j) {
  return (i >= 0) && (i <= 7) && (j >= 0) && (j <= 7);
}
