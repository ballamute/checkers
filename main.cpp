#include "checkers.hpp"

//------------------------------------------------
//! Main function allowing to play checkers in
//! 2 human player mode
//!
//! @return Number of roots
//!
//! @note Three comands: move, eat and end.
//! move and eat requires coordinates as in params.
//------------------------------------------------
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
        while (player_1.MakeMove(from.i, from.j, to.i, to.j, game)
            == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      } else {
        while (player_2.MakeMove(from.i, from.j, to.i, to.j, game)
            == Mistakes::NOT_OK) {
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
        while (player_1.Eat(from.i, from.j, to.i, to.j, game)
            == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      } else {
        while (player_2.Eat(from.i, from.j, to.i, to.j, game)
            == Mistakes::NOT_OK) {
          cout << "Type another coordinates here:\n";
          cin >> from.i >> from.j >> to.i >> to.j;
        }
        system("clear");
        game.ShowField();
        ++turn;
      }
    } else if (cmd == "end") {
      break;
    } else {
      cout << "Wrong command\n";
    }
  }
}
