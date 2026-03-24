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
#include "variants/base.hxx"
#include "variants/standard.hxx"
#include <windows.h>
#include <conio.h>
#define KEY_DOWN(VK) ((GetAsyncKeyState(VK) & 0x8000) ? 1 : 0)

namespace standard {
  using namespace standardBase;
  
  bool checkMate() {
    if(!check()) return 0;
    int tmp = check(), startX = -1, startY = -1;
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        if(board[i][j].belong) {
          for(int k = 0; k < 8; k++) {
            for(int l = 0; l < 8; l++) {
              if(checkMove(i, j, k, l) && board[i][j].belong == tmp) {
                piece tmp = board[k][l], ttmp = board[i][j];
                board[k][l] = board[i][j];
                board[k][l].moved = 1;
                board[i][j] = {'\0', 0, 0};
                bool f = !check();
                board[i][j] = board[k][l];
                board[k][l] = tmp;
                if(f) return 0;
              }
            }
          }
        }
      }
    }
    std::cout << "wtf" << '\n';
    return 1;
  }
  
  bool staleMate() {
    if(check()) return 0;
    for(int i = 0; i < 8; i++) {
      for(int j = 0; j < 8; j++) {
        if(board[i][j].belong) {
          for(int k = 0; k < 8; k++) {
            for(int l = 0; l < 8; l++) {
              if(checkMove(i, j, k, l) && board[i][j].belong == currentMove) return 0;
            }
          }
        }
      }
    }
    return 1;
  }
  
  char promote(int x, int y) {
    std::cout << "Choose the piece that the pawn is going to be promote to: ";
    while(1) {
      bool f = 0;
      if(KEY_DOWN('N')) board[x][y].type = 'N';
      else if(KEY_DOWN('B')) board[x][y].type = 'B';
      else if(KEY_DOWN('R')) board[x][y].type = 'R';
      else if(KEY_DOWN('Q')) board[x][y].type = 'Q';
      else f = 1;
      Sleep(50);
      if(!f) break;
    }
    return board[x][y].type;
  }
  
  void capture(int startX, int startY, int endX, int endY) {
    board[endX][endY] = {'\0', 0, 0};
  }
  
  void move(int startX, int startY, int endX, int endY) {
    if(!checkMove(startX, startY, endX, endY)) return ;
    currentMove *= -1;
    std::string result = "";
    pass();
    if(board[startX][startY].type == 'K' && startX == endX && abs(startY - endY) == 2) { // Castle
      if(startY > endY) { // O-O-O
        board[endX][endY] = board[startX][startY], board[endX][endY + 1] = board[7 * (currentMove == -1)][0];
        board[startX][startY] = board[7 * (currentMove == -1)][0] = {'\0', 0, 0};
        board[endX][endY].moved = board[endX][endY + 1].moved = 1;
        result = "O-O-O";
      }else {
        board[endX][endY] = board[startX][startY], board[endX][endY - 1] = board[7 * (currentMove == -1)][7];
        board[startX][startY] = board[7 * (currentMove == -1)][7] = {'\0', 0, 0};
        board[endX][endY].moved = board[endX][endY - 1].moved = 1;
        result = "O-O";
      }
    }else {
      bool passant = 0;
      if(board[startX][startY].type == 'P'
      && !board[endX][endY].belong
      && board[endX - currentMove][endY].belong != board[startX][startY].belong
      && endX == startX + currentMove && abs(startY - endY) == 1) passant = 1, capture(startX, startY, endX - currentMove, endY);
      if(board[startX][startY].type != 'P') result += board[startX][startY].type;
      if(board[endX][endY].belong || passant) {
        if(board[endX][endY].belong) capture(startX, startY, endX, endY);
        result += 'x';
      }
      result += char(endY + 'a');
      result += char(8 - endX + '0');
      board[endX][endY] = board[startX][startY];
      board[startX][startY] = {'\0', 0, 0};
      board[endX][endY].moved = 1;
      if(board[endX][endY].type == 'P' && endY == startY && abs(endX - startX) == 2) {
        board[endX][endY].moved = 2; // en passant avaliable
      }
      if(checkMate()) result += '#';
      else if(staleMate()) result += '@';
      else if(check()) result += '+';
      if(board[endX][endY].type == 'P' && (board[endX][endY].belong > 0 && !endX) || (board[endX][endY].belong < 0 && endX == 7)) result += promote(endX, endY);
    }
    game.push_back(result);
  }
}
