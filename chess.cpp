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
// Singleplayer Chess v1
// Invite your friends to have matches with each other!
/**
 * Embedded changelog:
 * v0 - Intital Software
 * - finished basic movements.
 * - due to lack of time, didn't make O-O(-O) and promotion.
 * - many bugs intended.
 * v0 patch 1
 * - fixed a bug where pawns can move 2 blocks forward+capture.
 * - fixed a bug where coordinate goes inversed in rows.
 * v0.1 - Castle and Promote
 * - Added O-O.
 * - Added promotion(still no en passant).
 * v1 - Finally, chess!
 * - Added en passant.
 * - Normal chess complete!
 * v1 patch 1
 * - fixed a bug where pawns cannot move normally.
 * - splitted the game into different files for convience of further development.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "variants.hxx"
#define popc __builtin_popcountll
#define ctz __builtin_ctzll
#define KEY_DOWN(VK) ((GetAsyncKeyState(VK) & 0x8000) ? 1 : 0)
using namespace std;
using namespace standard;
using ll = long long;

int wx, wy, bx, by, selx, sely, selected;

signed main() {
  init();
  while(1) {
    system("cls");
    print();
    cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
    cout << "You are currently hovering on ";
    if(currentMove > 0) cout << char(wy + 'a') << 8 - wx;
    else cout << char(by + 'a') << 8 - bx;
    if(selected) cout << ", you are currently selecting " << char(sely + 'a') << 8 - selx;
    cout << endl;
    while(1) {
      bool f = 1;
      if(KEY_DOWN('A') || KEY_DOWN(VK_LEFT)) {
        if(currentMove > 0) wy = max(wy - 1, 0);
        else by = max(by - 1, 0);
      }else if(KEY_DOWN('D') || KEY_DOWN(VK_RIGHT)) {
        if(currentMove > 0) wy = min(wy + 1, 7);
        else by = min(by + 1, 7);
      }else if(KEY_DOWN('W') || KEY_DOWN(VK_UP)) {
        if(currentMove > 0) wx = max(wx - 1, 0);
        else bx = max(bx - 1, 0);
      }else if(KEY_DOWN('S') || KEY_DOWN(VK_DOWN)) {
        if(currentMove > 0) wx = min(wx + 1, 7);
        else bx = min(bx + 1, 7);
      }else if(KEY_DOWN('Q') || KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN)) {
        int cx = 0, cy = 0;
        if(currentMove > 0) cx = wx, cy = wy;
        else cx = bx, cy = by;
        if(!selected) {
          if(board[cx][cy].belong == currentMove) selected = 1, selx = cx, sely = cy;
        }else {
          if(checkMove(selx, sely, cx, cy)) move(selx, sely, cx, cy);
          selected = 0, selx = 0, sely = 0;
        }
      }else f = 0;
      Sleep(50);
      if(f) break;
    }
    if(checkMate() || staleMate()) {
      system("cls");
      print();
      cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
      cout << "You are currently hovering on ";
      if(currentMove > 0) cout << char(wy + 'a') << 8 - wx;
      else cout << char(by + 'a') << 8 - bx;
      if(selected) cout << ", you are currently selecting " << char(sely + 'a') << 8 - selx;
      cout << endl;
      break;
    }
  }
  for(auto i : game) cout << i << ' ';
  cout << endl;
  return 0;
}
