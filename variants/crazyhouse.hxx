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

#ifndef CRAZYHOUSE_BASE
#define CRAZYHOUSE_BASE
namespace crazyhouseBase {
  int whitePocket[5], blackPocket[5], typeMap[5] = {'P', 'N', 'B', 'R', 'Q'};
  
  int transIndex(char c) {
    switch(c) {
    case 'P':
      return 0;
    case 'N':
      return 1;
    case 'B':
      return 2;
    case 'R':
      return 3;
    case 'Q':
      return 4;
    default:
      return -1;
    }
    return -1;
  }
  
  void init() {
    for(int i = 0; i < 8; i++) board[0][i] = {baseRow[i], 0, -1}, board[1][i] = {'P', 0, -1}, board[6][i] = {'P', 0, 1}, board[7][i] = {baseRow[i], 0, 1};
    save();
  }
  
  void pass() { // for en passant
    for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++) board[i][j].moved -= (board[i][j].moved >= 2);
  }
  
  bool checkPawn(int startX, int startY, int endX, int endY) {
    if(board[startX][startY].belong == board[endX][endY].belong) return 0;
    if(board[endX][endY].belong && endX == startX - board[startX][startY].belong && abs(endY - startY) == 1) return 1; // capture
    if(endX == startX - board[startX][startY].belong 
       && board[endX + currentMove][endY].belong != board[startX][startY].belong 
       && abs(endY - startY) == 1 && board[endX + currentMove][endY].moved == 2) return 1; // en passant
    if(board[endX][endY].belong) return 0;
    if(startY != endY) return 0;
    if(board[startX][startY].belong == 1) {
      if(endX > startX) return 0;
    }else {
      if(endX < startX) return 0;
    }
    return abs(endX - startX) <= 1 + !board[endX][endY].moved;
  }
  bool checkRook(int startX, int startY, int endX, int endY) {
    if(board[startX][startY].belong == board[endX][endY].belong) return 0;
    if(startX != endX && startY != endY) return 0;
    if(startX == endX) {
      for(int i = min(startY, endY) + 1; i <= max(startY, endY) - 1; i++) if(board[startX][i].belong) return 0;
    }else {
      for(int i = min(startX, endX) + 1; i <= max(startX, endX) - 1; i++) if(board[i][startY].belong) return 0;
    }
    return 1;
  }
  bool checkKnight(int startX, int startY, int endX, int endY) {
    if(board[startX][startY].belong == board[endX][endY].belong) return 0;
    int xx = abs(startX - endX), yy = abs(startY - endY);
    return (xx == 2 && yy == 1) || (xx == 1 && yy == 2);
  }
  bool checkBishop(int startX, int startY, int endX, int endY) {
    if(board[startX][startY].belong == board[endX][endY].belong) return 0;
    if(startX + startY == endX + endY) {
      for(int i = min(startX, endX) + 1, j = max(startY, endY) - 1; i <= max(startX, endX) - 1; i++, j--) if(board[i][j].belong) return 0;
      return 1;
    }else if(startX - startY == endX - endY) {
      for(int i = min(startX, startY) + 1, j = min(startY, endY) + 1; i <= max(startX, endX) - 1; i++, j++) if(board[i][j].belong) return 0;
      return 1;
    }
    return 0;
  }
  bool checkQueen(int startX, int startY, int endX, int endY) {return checkRook(startX, startY, endX, endY) || checkBishop(startX, startY, endX, endY);}
  
  bool checkValuable(int startX, int startY, int endX, int endY) {
    if(!board[startX][startY].belong) return 0;
    switch(board[startX][startY].type) {
    case 'P':
      return checkPawn(startX, startY, endX, endY);
    case 'R':
      return checkRook(startX, startY, endX, endY);
    case 'N':
      return checkKnight(startX, startY, endX, endY);
    case 'B':
      return checkBishop(startX, startY, endX, endY);
    case 'Q':
      return checkQueen(startX, startY, endX, endY);
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
  
  bool checkKing(int startX, int startY, int endX, int endY) {
    if(!(max(abs(endX - startX), abs(endY - startY)) == 1 && board[startX][startY].belong != board[endX][endY].belong)) return 0;
    return 1;
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
  
  void capture(int startX, int startY, int endX, int endY, int source, int multiplier = 1) {
    if(source == 1) whitePocket[transIndex(board[endX][endY].type)] += multiplier;
    else blackPocket[transIndex(board[endX][endY].type)] += multiplier;
    board[endX][endY] = {'\0', 0, 0};
  }
  
  void _move(int startX, int startY, int endX, int endY) {
    pass();
    if(board[startX][startY].type == 'K' && startX == endX && abs(startY - endY) == 2) {
      if(startY > endY) {
        board[endX][endY] = board[startX][startY], board[endX][endY + 1] = board[7 * (currentMove == -1)][0];
        board[startX][startY] = board[7 * (currentMove == -1)][0] = {'\0', 0, 0};
        board[endX][endY].moved = board[endX][endY + 1].moved = 1;
      }else {
        board[endX][endY] = board[startX][startY], board[endX][endY - 1] = board[7 * (currentMove == -1)][7];
        board[startX][startY] = board[7 * (currentMove == -1)][7] = {'\0', 0, 0};
        board[endX][endY].moved = board[endX][endY - 1].moved = 1;
      }
    }else {
      bool passant = 0;
      if(board[startX][startY].type == 'P'
         && !board[endX][endY].belong
         && board[endX - currentMove][endY].belong != board[startX][startY].belong
         && endX == startX + currentMove && abs(startY - endY) == 1) passant = 1, capture(startX, startY, endX + currentMove, endY, currentMove, 0);
      if(board[endX][endY].belong || passant) {
        if(board[endX][endY].belong) capture(startX, startY, endX, endY, currentMove, 0);
      }
      if(board[endX][endY].type == 'P' && endY == startY && abs(endX - startX) == 2) board[endX][endY].moved = 2;
      board[endX][endY] = board[startX][startY];
      board[startX][startY] = {'\0', 0, 0};
      if(board[endX][endY].belong) board[endX][endY].moved = 1;
      if(board[endX][endY].type == 'P' && endY == startY && abs(endX - startX) == 2) board[endX][endY].moved = 2;
    }
  }
  
  bool checkMove(int startX, int startY, int endX, int endY) {
    if(!board[startX][startY].belong) return 0;
    bool f = 1;
    switch(board[startX][startY].type) {
    case 'P':
      f &= checkPawn(startX, startY, endX, endY);
      break;
    case 'R':
      f &= checkRook(startX, startY, endX, endY);
      break;
    case 'N':
      f &= checkKnight(startX, startY, endX, endY);
      break;
    case 'B':
      f &= checkBishop(startX, startY, endX, endY);
      break;
    case 'Q':
      f &= checkQueen(startX, startY, endX, endY);
      break;
    case 'K':
      bool g = checkKing(startX, startY, endX, endY);
      if(startX == endX && abs(startY - endY) == 2) { // Consider Castle
        if(startY > endY) g |= checkCastle(7 * (currentMove == 1), 0);
        else g |= checkCastle(7 * (currentMove == 1), 7);
      }
      f &= g;
      break;
    }
    if(f) {
      _move(startX, startY, endX, endY);
      f &= check() != currentMove;
      revert();
    }
    return f;
  }
  
  bool checkPlace(int x, int y, char type) {
    if(type == 'P' && (x == 0 || x == 7)) return 0;
    if(board[x][y].belong) return 0;
    if(currentMove == 1) return whitePocket[transIndex(type)];
    else return blackPocket[transIndex(type)];
  }
  
  void _place(int x, int y, char type) {
    if(!checkPlace(x, y, type)) return ;
    pass();
    board[x][y] = {type, 1, currentMove};
  }
}
#endif
