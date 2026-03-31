/**
 * This file is a part of Singleplayer Chess.
 * Singleplayer Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published blackY the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 * 
 * Singleplayer Chess is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with Singleplayer Chess.
 * If not, see <https://www.gnu.org/licenses/>.
 */
// Singleplayer Chess v2
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
 * - added O-O.
 * - added promotion(still no en passant).
 * v1 - Finally, chess!
 * - added en passant.
 * - Standard complete!
 * v1 patch 1
 * - fixed a bug where pawns cannot move normally.
 * - splitted the game into different files for convience of further development.
 * v1.1 - Nuclear bomb
 * - added Atomic.
 * - probably need an update for the main screen.
 * v1.1 patch 1
 * - fixed a bug where checking other's king is not allowed.
 * - fixed a bug where rook can go thru pawns.
 * v2 - Power of Communist
 * - added the main screen(yes, I delayed the development of it until now, because chess with only 1 variant doesn't need main screen).
 * - added Crazyhouse.
 * - fixed a bug in the checking of en passant.
 */
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#include "variants.hxx"
#define popc __builtin_popcountll
#define ctz __builtin_ctzll
#define KEY_DOWN(VK) ((GetAsyncKeyState(VK) & 0x8000) ? 1 : 0)
using namespace std;
using namespace base;
using ll = long long;

int whiteX, whiteY, blackX, blackY, selectX, selectY, selected;

void mainScreen() {
  cout << "+--------------------------------------------------------------------------+" << endl;
  cout << "|                                                                          |" << endl;
  cout << "|      Welcome to Singleplayer Chess v2, a chess game libre software.      |" << endl;
  cout << "|                                                                          |" << endl;
  cout << "+--------------------------------------------------------------------------+" << endl;
  cout << endl;
  cout << endl;
  cout << "Press S to play Standard Chess." << endl;
  cout << "Press C to play Crazyhouse Chess." << endl;
  cout << "Press A to play Atomic Chess." << endl;
}

void standardChess() {
  standardBase::init();
  while(1) {
    system("cls");
    print();
    cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
    cout << "You are currently hovering on ";
    if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
    else cout << char(blackY + 'a') << 8 - blackX;
    if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
    cout << endl;
    while(1) {
      bool f = 1;
      if(KEY_DOWN('A') || KEY_DOWN(VK_LEFT)) {
        if(currentMove > 0) whiteY = max(whiteY - 1, 0);
        else blackY = max(blackY - 1, 0);
      }else if(KEY_DOWN('D') || KEY_DOWN(VK_RIGHT)) {
        if(currentMove > 0) whiteY = min(whiteY + 1, 7);
        else blackY = min(blackY + 1, 7);
      }else if(KEY_DOWN('W') || KEY_DOWN(VK_UP)) {
        if(currentMove > 0) whiteX = max(whiteX - 1, 0);
        else blackX = max(blackX - 1, 0);
      }else if(KEY_DOWN('S') || KEY_DOWN(VK_DOWN)) {
        if(currentMove > 0) whiteX = min(whiteX + 1, 7);
        else blackX = min(blackX + 1, 7);
      }else if(KEY_DOWN('Q') || KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN)) {
        int cx = 0, cy = 0;
        if(currentMove > 0) cx = whiteX, cy = whiteY;
        else cx = blackX, cy = blackY;
        if(!selected) {
          if(board[cx][cy].belong == currentMove) selected = 1, selectX = cx, selectY = cy;
        }else {
          if(standardBase::checkMove(selectX, selectY, cx, cy)) standardX::move(selectX, selectY, cx, cy);
          selected = 0, selectX = 0, selectY = 0;
        }
      }else f = 0;
      Sleep(50);
      if(f) break;
    }
    if(standardX::checkMate() || standardX::staleMate()) {
      system("cls");
      print();
      cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
      cout << "You are currently hovering on ";
      if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
      else cout << char(blackY + 'a') << 8 - blackX;
      if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
      cout << endl;
      break;
    }
  }
  for(auto i : game) cout << i << ' ';
  cout << endl;
  system("Pause");
}
void crazyhouseChess() {
  crazyhouseBase::init();
  while(1) {
    system("cls");
    print();
    cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
    cout << "You are currently hovering on ";
    if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
    else cout << char(blackY + 'a') << 8 - blackX;
    if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
    cout << endl;
    cout << "White has ";
    cout << crazyhouseBase::whitePocket[0] << " Pawns, ";
    cout << crazyhouseBase::whitePocket[1] << " Knights, ";
    cout << crazyhouseBase::whitePocket[2] << " Bishops, ";
    cout << crazyhouseBase::whitePocket[3] << " Rooks, ";
    cout << crazyhouseBase::whitePocket[4] << " Queens ";
    cout << "in his pocket" << endl;
    cout << "Black has ";
    cout << crazyhouseBase::blackPocket[0] << " Pawns, ";
    cout << crazyhouseBase::blackPocket[1] << " Knights, ";
    cout << crazyhouseBase::blackPocket[2] << " Bishops, ";
    cout << crazyhouseBase::blackPocket[3] << " Rooks, ";
    cout << crazyhouseBase::blackPocket[4] << " Queens ";
    cout << "in his pocket" << endl;
    while(1) {
      bool f = 1;
      if(KEY_DOWN('A') || KEY_DOWN(VK_LEFT)) {
        if(currentMove > 0) whiteY = max(whiteY - 1, 0);
        else blackY = max(blackY - 1, 0);
      }else if(KEY_DOWN('D') || KEY_DOWN(VK_RIGHT)) {
        if(currentMove > 0) whiteY = min(whiteY + 1, 7);
        else blackY = min(blackY + 1, 7);
      }else if(KEY_DOWN('W') || KEY_DOWN(VK_UP)) {
        if(currentMove > 0) whiteX = max(whiteX - 1, 0);
        else blackX = max(blackX - 1, 0);
      }else if(KEY_DOWN('S') || KEY_DOWN(VK_DOWN)) {
        if(currentMove > 0) whiteX = min(whiteX + 1, 7);
        else blackX = min(blackX + 1, 7);
      }else if(KEY_DOWN('Q') || KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN)) {
        int cx = 0, cy = 0;
        if(currentMove > 0) cx = whiteX, cy = whiteY;
        else cx = blackX, cy = blackY;
        if(!selected) {
          selected = 1, selectX = cx, selectY = cy;
        }else {
          if(crazyhouseBase::checkMove(selectX, selectY, cx, cy)) crazyhouseX::move(selectX, selectY, cx, cy);
          selected = 0, selectX = 0, selectY = 0;
        }
      }else if(KEY_DOWN(VK_LSHIFT) && KEY_DOWN('2')) {
        if(selected) {
          char type = '\0';
          cout << "Choose the piece you want to place: " << endl;
          while(1) {
            bool f = 1;
            if(KEY_DOWN('P')) type = 'P';
            else if(KEY_DOWN('N')) type = 'N';
            else if(KEY_DOWN('B')) type = 'B';
            else if(KEY_DOWN('R')) type = 'R';
            else if(KEY_DOWN('Q')) type = 'Q';
            else f = 0;
            Sleep(50);
            if(f) break;
          }
          if(crazyhouseBase::checkPlace(selectX, selectY, type)) crazyhouseX::place(selectX, selectY, type);
          selected = 0, selectX = 0, selectY = 0;
        }
      }else f = 0;
      Sleep(50);
      if(f) break;
    }
    if(crazyhouseX::checkMate() || crazyhouseX::staleMate()) {
      system("cls");
      print();
      cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
      cout << "You are currently hovering on ";
      if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
      else cout << char(blackY + 'a') << 8 - blackX;
      if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
      cout << endl;
      cout << "White has ";
      cout << crazyhouseBase::whitePocket[0] << " Pawns, ";
      cout << crazyhouseBase::whitePocket[1] << " Knights, ";
      cout << crazyhouseBase::whitePocket[2] << " Bishops, ";
      cout << crazyhouseBase::whitePocket[3] << " Rooks, ";
      cout << crazyhouseBase::whitePocket[4] << " Queens ";
      cout << "in his pocket" << endl;
      cout << "Black has ";
      cout << crazyhouseBase::blackPocket[0] << " Pawns, ";
      cout << crazyhouseBase::blackPocket[1] << " Knights, ";
      cout << crazyhouseBase::blackPocket[2] << " Bishops, ";
      cout << crazyhouseBase::blackPocket[3] << " Rooks, ";
      cout << crazyhouseBase::blackPocket[4] << " Queens ";
      cout << "in his pocket" << endl;
      break;
    }
  }
  for(auto i : game) cout << i << ' ';
  cout << endl;
  system("Pause");
}
void atomicChess() {
  atomicBase::init();
  while(1) {
    system("cls");
    print();
    cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
    cout << "You are currently hovering on ";
    if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
    else cout << char(blackY + 'a') << 8 - blackX;
    if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
    cout << endl;
    while(1) {
      bool f = 1;
      if(KEY_DOWN('A') || KEY_DOWN(VK_LEFT)) {
        if(currentMove > 0) whiteY = max(whiteY - 1, 0);
        else blackY = max(blackY - 1, 0);
      }else if(KEY_DOWN('D') || KEY_DOWN(VK_RIGHT)) {
        if(currentMove > 0) whiteY = min(whiteY + 1, 7);
        else blackY = min(blackY + 1, 7);
      }else if(KEY_DOWN('W') || KEY_DOWN(VK_UP)) {
        if(currentMove > 0) whiteX = max(whiteX - 1, 0);
        else blackX = max(blackX - 1, 0);
      }else if(KEY_DOWN('S') || KEY_DOWN(VK_DOWN)) {
        if(currentMove > 0) whiteX = min(whiteX + 1, 7);
        else blackX = min(blackX + 1, 7);
      }else if(KEY_DOWN('Q') || KEY_DOWN(VK_SPACE) || KEY_DOWN(VK_RETURN)) {
        int cx = 0, cy = 0;
        if(currentMove > 0) cx = whiteX, cy = whiteY;
        else cx = blackX, cy = blackY;
        if(!selected) {
          if(board[cx][cy].belong == currentMove) selected = 1, selectX = cx, selectY = cy;
        }else {
          if(atomicBase::checkMove(selectX, selectY, cx, cy)) atomicX::move(selectX, selectY, cx, cy);
          selected = 0, selectX = 0, selectY = 0;
        }
      }else f = 0;
      Sleep(50);
      if(f) break;
    }
    if(atomicX::checkMate() || atomicX::staleMate()) {
      system("cls");
      print();
      cout << "Now is " << (currentMove > 0? "White" : "Black") << "'s turn. ";
      cout << "You are currently hovering on ";
      if(currentMove > 0) cout << char(whiteY + 'a') << 8 - whiteX;
      else cout << char(blackY + 'a') << 8 - blackX;
      if(selected) cout << ", you are currently selecting " << char(selectY + 'a') << 8 - selectX;
      cout << endl;
      break;
    }
  }
  for(auto i : game) cout << i << ' ';
  cout << endl;
  system("Pause");
}

signed main() {
  while(1) {
    system("cls");
    mainScreen();
    char c = '\0';
    while(1) {
      bool f = 1;
      if(KEY_DOWN('S')) c = 'S';
      else if(KEY_DOWN('C')) c = 'C';
      else if(KEY_DOWN('A')) c = 'A';
      else f = 0;
      Sleep(50);
      if(f) break;
    }
    if(c == 'S') standardChess();
    else if(c == 'C') crazyhouseChess();
    else atomicChess();
  }
  return 0;
}
