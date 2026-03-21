// Singleplayer Chess v1.2
// Invite your friends to have matches with each other!
/**
 * Embedded changelog:
 * v1 - Intital Software
 * - finished basic movements.
 * - due to lack of time, didn't make O-O(-O) and promotion.
 * - many bugs intended.
 * v1 patch 1
 * - fixed a bug where pawns can move 2 blocks forward wo capture.
 * - fixed a bug where rook can move left.
 * - fixed a bug where the PGN goes inersed in rows.
 * v1.1 - Castle and Promote
 * - Added O-O.
 * - Added promotion(still no en passant).
 * v1.2 - Finally, chess!
 * - Added en passant.
 * - Normal chess complete!
 */
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
#define int ll
#define popc __builtin_popcountll
#define ctz __builtin_ctzll
#define ldouble long double
#define DEBUG
#define KEY_DOWN(VK) ((GetAsyncKeyState(VK) & 0x8000) ? 1 : 0)
using namespace std;
using ll = long long;

const char baseRow[8] = {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'};

struct piece {
  char type;
  int moved;
  int belong;
  // moved tag is to check for castling and pawn's first move
}board[8][8];

int bx, by, wx, wy, currentMove = 1, selected, selx, sely;
vector<string> game;

ostream& operator <<(ostream &a, piece x) {
  if(x.belong < 0) cout << 'B';
  else if(x.belong > 0) cout << 'W';
  else cout << '.';
  switch(x.type) {
  case 'P':
    cout << '#';
    break;
  case 'R':
    cout << 'R';
    break;
  case 'N':
    cout << 'N';
    break;
  case 'B':
    cout << 'B';
    break;
  case 'Q':
    cout << 'Q';
    break;
  case 'K':
    cout << '+';
    break;
  default:
    cout << '.';
  }
  return a;
}

void print() {
  for(int i = 0; i < 8; i++) {
    for(int j = 0; j < 8; j++) cout << board[i][j] << ' ';
    cout << '\n';
  }
}

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
  if(board[sx][sy].belong == 1) if(ex > sx) return 0;
  else if(ex < sx) return 0;
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

bool checkMate() {
  if(!check()) return 0;
  int tmp = check(), sx = -1, sy = -1;
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

void move(int sx, int sy, int ex, int ey) {
  if(!checkMove(sx, sy, ex, ey)) return ;
  currentMove *= -1;
  string fuck = "";
  pass();
  if(board[sx][sy].type == 'K' && sx == ex && abs(sy - ey) == 2) { // Castle
    if(sy > ey) { // O-O-O
      board[ex][ey] = board[sx][sy], board[ex][ey + 1] = board[7 * (currentMove == -1)][0];
      board[sx][sy] = board[7 * (currentMove == -1)][0] = {'\0', 0, 0};
      board[ex][ey].moved = board[ex][ey + 1].moved = 1;
      fuck = "O-O-O";
    }else {
      board[ex][ey] = board[sx][sy], board[ex][ey - 1] = board[7 * (currentMove == -1)][7];
      board[sx][sy] = board[7 * (currentMove == -1)][7] = {'\0', 0, 0};
      board[ex][ey].moved = board[ex][ey - 1].moved = 1;
      fuck = "O-O";
    }
  }else {
    if(board[sx][sy].type == 'P' && !board[ex][ey].belong && board[ex - currentMove][ey].belong != board[sx][sy].belong && ex == sx + currentMove && abs(sy - ey) == 1) board[ex - currentMove][ey] = {'\0', 0, 0};
    if(board[sx][sy].type != 'P') fuck += board[sx][sy].type;
    if(board[ex][ey].belong) fuck += 'x';
    fuck += char(ey + 'a');
    fuck += char(8 - ex + '0');
    board[ex][ey] = board[sx][sy];
    board[sx][sy] = {'\0', 0, 0};
    board[ex][ey].moved = 1;
    if(board[ex][ey].type == 'P' && ey == sy && abs(ex - sx) == 2) {
      board[ex][ey].moved = 2; // en passant avaliable
    }
    if(checkMate()) fuck += '#';
    else if(staleMate()) fuck += '@';
    else if(check()) fuck += '+';
    if(board[ex][ey].type == 'P' && (board[ex][ey].belong > 0 && !ex) || (board[ex][ey].belong < 0 && ex == 7)) {
      cout << "Choose the piece that the pawn is going to be promote to: ";
      while(1) {
        bool f = 0;
        if(KEY_DOWN('N')) board[ex][ey].type = 'N';
        else if(KEY_DOWN('B')) board[ex][ey].type = 'B';
        else if(KEY_DOWN('R')) board[ex][ey].type = 'R';
        else if(KEY_DOWN('Q')) board[ex][ey].type = 'Q';
        else f = 1;
        Sleep(50);
        if(!f) break;
      }
    }
  }
  game.push_back(fuck);
}

signed main() {
#ifndef DEBUG
  ios::sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
#endif
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
        if(currentMove > 0) wy = max(wy - 1, 0ll);
        else by = max(by - 1, 0ll);
      }else if(KEY_DOWN('D') || KEY_DOWN(VK_RIGHT)) {
        if(currentMove > 0) wy = min(wy + 1, 7ll);
        else by = min(by + 1, 7ll);
      }else if(KEY_DOWN('W') || KEY_DOWN(VK_UP)) {
        if(currentMove > 0) wx = max(wx - 1, 0ll);
        else bx = max(bx - 1, 0ll);
      }else if(KEY_DOWN('S') || KEY_DOWN(VK_DOWN)) {
        if(currentMove > 0) wx = min(wx + 1, 7ll);
        else bx = min(bx + 1, 7ll);
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
