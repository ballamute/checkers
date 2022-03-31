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

class Cell {
 public:
  Coordinates loc;
  CellCondition cond;
  bool king_on;
};

bool InField(int i, int j) {
  return (i >= 0) && (i <= 7) && (j >= 0) && (j <= 7);
}

class GameField {
 public:
  GameField() {
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
  ~GameField() = default;
  void ShowField() {
    //system("clear");
    cout << "|||0 1 2 3 4 5 6 7\n";
    cout << "||||||||||||||||||\n";
    for (int i = 0; i < 8; ++i) {
      cout << i << "||";
      for (int j = 0; j < 8; ++j) {
        cout << (int)field[i][j].cond << ' '; // 0 - empty, 1 - white, 2 - black
      }
      cout << '\n';
    }
    cout << "-------------------------------" << '\n';
  }

  Moves GetMoves(int loc_i, int loc_j) {
    Moves moves;
    Cell cell = field[loc_i][loc_j];
    CellCondition enemy_sell_cond;
    int sign = -1;
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
        k = 1;
      }
    }

    if (InField(cell.loc.i + sign, cell.loc.j - 1)) {
      if (field[cell.loc.i + sign][cell.loc.j - 1].cond == CellCondition::EMPTY) {
        moves.to_move.push_back({cell.loc.i + sign, cell.loc.j - 1});
      } else if (field[cell.loc.i + sign][cell.loc.j - 1].cond == enemy_sell_cond &&
          field[cell.loc.i + 2 * sign][cell.loc.j - 2].cond == CellCondition::EMPTY) {
        moves.to_eat.push_back({cell.loc.i + 2 * sign, cell.loc.j - 2});
      }
    }
    if (InField(cell.loc.i + sign, cell.loc.j + 1)) {
      if (field[cell.loc.i + sign][cell.loc.j + 1].cond
          == CellCondition::EMPTY) {
        moves.to_move.push_back({cell.loc.i + sign, cell.loc.j + 1});
      } else if (field[cell.loc.i + sign][cell.loc.j + 1].cond == enemy_sell_cond &&
          field[cell.loc.i + 2 * sign][cell.loc.j + 2].cond == CellCondition::EMPTY) {
        moves.to_eat.push_back({cell.loc.i + 2 * sign, cell.loc.j + 2});
      }
    }
    return moves;
  }

  vector<vector<Cell>> field;
};



class Player {
 public:
  explicit Player(PlayerSide player_side) {
    side = player_side;
  }
  ~Player() = default;

  Mistakes MakeMove(int prev_i, int prev_j, int i, int j, GameField& game_field) const {
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
    } else if ((int)game_field.field[prev_i][prev_j].cond != (int)side) {
      cout << "You can`t touch your opponent`s checker\n";
      return Mistakes::NOT_OK;
    }

    for (int it_1 = 0; it_1 < 8; it_1++) {
      for (int it_2 = 0; it_2 < 8; it_2++) {
        if (!game_field.GetMoves(it_1, it_2).to_eat.empty() &&
            game_field.field[prev_j][prev_j].cond == game_field.field[it_1][it_2].cond) {
          cout << "You have to eat now!\n";
          return Mistakes::NOT_OK;
        }
      }
    }
    vector<Coordinates> to_move = game_field.GetMoves(prev_i, prev_j).to_move;
    vector<Coordinates> to_eat = game_field.GetMoves(prev_i, prev_j).to_eat;


    bool is_moving = false;
    for (auto & it : to_move) {
      if (it.i == i && it.j == j) {
        is_moving = true;
      }
    }
    if (!is_moving) {
      cout << "Move is not allowed by the rules. "
              "The amount of the cells you can move on is "
              << to_move.size() << '\n';
      for (auto & el : to_move) {
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


  Mistakes Eat(int prev_i, int prev_j, int i, int j, GameField& game_field) {
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
    } else if ((int)game_field.field[prev_i][prev_j].cond != (int)side) {
      cout << "You can`t touch your opponent`s checker\n";
      return Mistakes::NOT_OK;
    }
    vector<Coordinates> to_eat = game_field.GetMoves(prev_i, prev_j).to_eat;
    bool is_eating = false;
    for (auto & it : to_eat) {
      if (it.i == i && it.j == j) {
        is_eating = true;
      }
    }
    if (!is_eating) {
      cout << "This move cannot be called eating. "
              "The amount of the cells where you can it by going on them is "
           << to_eat.size() << '\n';
      for (auto & el : to_eat) {
        cout << el.i << ' ' << el.j << '\n';
      }
      return Mistakes::NOT_OK;
    }

    game_field.field[i][j].cond = game_field.field[prev_i][prev_j].cond;
    game_field.field[i][j].king_on = game_field.field[prev_i][prev_j].king_on;

    int i_iter = i < prev_i ? -1 : 1;
    int j_iter = j < prev_j ? -1 : 1;

    while(i != prev_i && j != prev_j) {
      game_field.field[prev_i][prev_j].cond = CellCondition::EMPTY;
      game_field.field[prev_i][prev_j].king_on = false;
      prev_i += i_iter;
      prev_j += j_iter;
    }
    return Mistakes::OK;
  }

  PlayerSide side;
};


int main() {
  GameField game;
  Player player_1(PlayerSide::WHITE);
  Player player_2(PlayerSide::BLACK);
  int turn = 1;
  system("clear");
  game.ShowField();
  while (true) {
    cout << ((turn % 2 == 1) ? "First player turn\n" : "Second player turn\n");
    std::string cmd;
    cin >> cmd;
    Coordinates from{};
    Coordinates to{};
    if (cmd == "move") {
      cin >> from.i >> from.j >> to.i >> to.j;
      if (turn % 2 == 1) {
        while (player_1.MakeMove(from.i, from.j, to.i, to.j, game) == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      } else {
        while (player_2.MakeMove(from.i, from.j, to.i, to.j, game) == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      }
    } else if (cmd == "eat") {
      cin >> from.i >> from.j >> to.i >> to.j;
      if (turn % 2 == 1) {
        while (player_1.Eat(from.i, from.j, to.i, to.j, game) == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      } else {
        while (player_2.Eat(from.i, from.j, to.i, to.j, game) == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      }
    } else if (cmd == "end") {
      break;
    }
  }
}
