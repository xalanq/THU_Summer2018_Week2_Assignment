#ifndef CHESS_H
#define CHESS_H

#include <vector>

namespace Chess {

enum Type {
    JIANG,
    SHI,
    XIANG,
    MA,
    JU,
    PAO,
    BING,
    NOTYPE
};

enum Color {
    RED,
    BLACK,
    NOCOLOR
};

struct Pos {
    int x, y;
    Pos(int x = 0, int y = 0) : x(x), y(y) {}
};

extern void clear();
extern void init();
extern bool checkWin(Color c);
extern bool checkJiang(Color c);
extern bool checkDuiJiang();
extern std::vector<Pos> possibleMove(int x, int y);
extern int moveTo(int x1, int y1, int x2, int y2, Color c); // -1, lose; 0: fail; 1: ok

extern Type type[9][10];
extern Color color[9][10];
extern Color winner;

extern void f(int i, int j, Type t, Color c);

}

#endif // CHESS_H
