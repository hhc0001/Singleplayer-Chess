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

#include "./base.hxx"
#include <utility>

using namespace base;
using std::min, std::max, std::swap;

#ifndef STANDARD_BASE
#define STANDARD_BASE
namespace standardBase {
  void init() {
    for(int i = 0; i < 8; i++) board[0][i] = {baseRow[i], 0, -1}, board[1][i] = {'P', 0, -1}, board[6][i] = {'P', 0, 1}, board[7][i] = {baseRow[i], 0, 1};
  }
  
  void pass() { // for en passant
    for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++) board[i][j].moved -= (board[i][j].moved >= 2);
  }
  
  bool checkPawn(int sx, int sy, int ex, int ey) {
    if(board[sx][sy].belong == board[ex][ey].belong) return 0;
    if(board[ex][ey].belong && ex == sx - board[sx][sy].belong && abs(ey - sy) == 1) return 1; // capture
    if(ex == sx - board[sx][sy].belong && board[ex + currentMove][ey].belong != board[sx][sy].belong && abs(ey - sy) == 1 && board[ex + currentMove][ey].moved == 2) return 1; // en passant
    if(board[ex][ey].belong) return 0;
    if(sy != ey) return 0;
    if(board[sx][sy].belong == 1) {
      if(ex > sx) return 0;
    }else {
      if(ex < sx) return 0;
    }
    return abs(ex - sx) <= 1 + !board[ex][ey].moved;
  }
  bool checkRook(int sx, int sy, int ex, int ey) {
    if(board[sx][sy].belong == board[ex][ey].belong) return 0;
    if(sx != ex && sy != ey) return 0;
    if(sx == ex) for(int i = min(sy, ey) + 1; i <= max(sy, ey) - 1; i++) if(board[sx][i].belong) return 0;
      else for(int i = min(sx, ex) + 1; i <= max(sx, ex) - 1; i++) if(board[i][sy].belong) return 0;
    return 1;
  }
  bool checkKnight(int sx, int sy, int ex, int ey) {
    if(board[sx][sy].belong == board[ex][ey].belong) return 0;
    int xx = abs(sx - ex), yy = abs(sy - ey);
    return (xx == 2 && yy == 1) || (xx == 1 && yy == 2);
  }
  bool checkBishop(int sx, int sy, int ex, int ey) {
    if(board[sx][sy].belong == board[ex][ey].belong) return 0;
    if(sx + sy == ex + ey) {
      for(int i = min(sx, ex) + 1, j = max(sy, ey) - 1; i <= max(sx, ex) - 1; i++, j--) if(board[i][j].belong) return 0;
      return 1;
    }else if(sx - sy == ex - ey) {
      for(int i = min(sx, sy) + 1, j = min(sy, ey) + 1; i <= max(sx, ex) - 1; i++, j++) if(board[i][j].belong) return 0;
      return 1;
    }
    return 0;
  }
  bool checkQueen(int sx, int sy, int ex, int ey) {return checkRook(sx, sy, ex, ey) || checkBishop(sx, sy, ex, ey);}
  
  bool checkValuable(int sx, int sy, int ex, int ey) {
    if(!board[sx][sy].belong) return 0;
    switch(board[sx][sy].type) {
    case 'P':
      return checkPawn(sx, sy, ex, ey);
    case 'R':
      return checkRook(sx, sy, ex, ey);
    case 'N':
      return checkKnight(sx, sy, ex, ey);
    case 'B':
      return checkBishop(sx, sy, ex, ey);
    case 'Q':
      return checkQueen(sx, sy, ex, ey);
    }
    return 0;
  }
  
  int check(int initial = currentMove) {
    bool fW = 0, fB = 0;
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        if(board[i][j].belong) {
          for(int k = 0; k < 8; k++) {
            for(int l = 0; l < 8; l++) {
              if(checkValuable(i, j, k, l) && board[k][l].type == 'K') {
                if(board[k][l].belong > 0) fW = 1;
                else fB = 1;
              }
            }
          }
        }
      }
    }
    if(fW || fB) {
      if(fW && fB) {
        return initial;
      }else if(fW) return 1;
      else return -1;
    }
    return 0;
  }
  
  bool checkKing(int sx, int sy, int ex, int ey) {
    if(!(max(abs(ex - sx), abs(ey - sy)) == 1 && board[sx][sy].belong != board[ex][ey].belong)) return 0;
    piece tmp = board[ex][ey], ttmp = board[sx][sy];
    board[ex][ey] = board[sx][sy];
    board[ex][ey].moved = 1;
    board[sx][sy] = {'\0', 0, 0};
    bool ret = !check();
    board[sx][sy] = ttmp;
    board[ex][ey] = tmp;
    return ret;
  }
  
  // Rook determines whether it is O-O or O-O-O
  bool checkCastle(int x, int y) {
    if(board[x][y].type != 'R') return 0;
    if(check()) return 0;
    int tx = 0, ty = 0;
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        if(board[i][j].type == 'K' && board[i][j].belong == currentMove) {
          tx = i, ty = j;
        }
      }
    }
    if(board[tx][ty].moved || board[x][y].moved) return 0;
    piece tmp = board[x][y], ttmp = board[tx][ty];
    board[tx][ty] = {'\0', 0, 0};
    bool f = checkRook(x, y, tx, ty);
    board[tx][ty] = ttmp;
    if(!f) return 0;
    if(y == 0) {
      swap(board[tx][ty], board[tx][ty - 1]);
      f &= check() != currentMove;
      swap(board[tx][ty], board[tx][ty - 1]);
      swap(board[tx][ty], board[tx][ty - 2]);
      f &= check() != currentMove;
      swap(board[tx][ty], board[tx][ty - 2]);
    }else {
      swap(board[tx][ty], board[tx][ty + 1]);
      f &= check() != currentMove;
      swap(board[tx][ty], board[tx][ty + 1]);
      swap(board[tx][ty], board[tx][ty + 2]);
      f &= check() != currentMove;
      swap(board[tx][ty], board[tx][ty + 2]);
    }
    return f;
  }
  
  bool checkMove(int sx, int sy, int ex, int ey) {
    if(!board[sx][sy].belong) return 0;
    bool f = 1;
    switch(board[sx][sy].type) {
    case 'P':
      f &= checkPawn(sx, sy, ex, ey);
      break;
    case 'R':
      f &= checkRook(sx, sy, ex, ey);
      break;
    case 'N':
      f &= checkKnight(sx, sy, ex, ey);
      break;
    case 'B':
      f &= checkBishop(sx, sy, ex, ey);
      break;
    case 'Q':
      f &= checkQueen(sx, sy, ex, ey);
      break;
    case 'K':
      bool g = checkKing(sx, sy, ex, ey);
      if(sx == ex && abs(sy - ey) == 2) { // Consider Castle
        if(sy > ey) g |= checkCastle(7 * (currentMove == 1), 0);
        else g |= checkCastle(7 * (currentMove == 1), 7);
      }
      f &= g;
      break;
    }
    if(f) {
      piece tmp = board[ex][ey], ttmp = board[sx][sy];
      board[ex][ey] = board[sx][sy];
      board[ex][ey].moved = 1;
      board[sx][sy] = {'\0', 0, 0};
      f &= check() != currentMove;
      board[sx][sy] = ttmp;
      board[ex][ey] = tmp;
    }
    return f;
  }
}
#endif
