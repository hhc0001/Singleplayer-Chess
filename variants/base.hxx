/**
 * This file is a part of Singleplayer Chess.
 * Singleplayer Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * 
 * Singleplayer Chess is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with Singleplayer Chess.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <vector>

#ifndef BASE
#define BASE
namespace base {
  const char baseRow[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}; // The lowmost row in STANDARD chess(useless in Chess960)
  
  struct piece {
    char type;
    int moved;
    int belong;
  }board[8][8];
  
  int currentMove = 1;
  std::vector<std::string> game;
  
  std::ostream& operator <<(std::ostream &a, piece x) {
    if(x.belong < 0) a << 'B';
    else if(x.belong > 0) a << 'W';
    else a << '.';
    switch(x.type) {
    case 'P':
      a << '#';
      break;
    case 'R':
      a << 'R';
      break;
    case 'N':
      a << 'N';
      break;
    case 'B':
      a << 'B';
      break;
    case 'Q':
      a << 'Q';
      break;
    case 'K':
      a << '+';
      break;
    default:
      a << '.';
    }
    return a;
  }
  
  void print() {
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) std::cout << board[i][j] << ' ';
      std::cout << '\n';
    }
  }
}
#endif
