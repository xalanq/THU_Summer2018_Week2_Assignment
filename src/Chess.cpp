#include "Chess.h"
#include <algorithm>

namespace Chess {

Type type[9][10];
Color color[9][10];

void clear() {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j) {
            type[i][j] = NOTYPE;
            color[i][j] = NOCOLOR;
        }
}

void init() {
    clear();

    f(4, 0, JIANG, RED);
    f(3, 0, SHI, RED); f(5, 0, SHI, RED);
    f(2, 0, XIANG, RED); f(6, 0, XIANG, RED);
    f(1, 0, MA, RED); f(7, 0, MA, RED);
    f(0, 0, JU, RED); f(8, 0, JU, RED);
    f(1, 2, PAO, RED); f(7, 2, PAO, RED);
    f(0, 3, BING, RED); f(2, 3, BING, RED); f(4, 3, BING, RED); f(6, 3, BING, RED); f(8, 3, BING, RED);

    f(4, 9, JIANG, BLACK);
    f(3, 9, SHI, BLACK); f(5, 9, SHI, BLACK);
    f(2, 9, XIANG, BLACK); f(6, 9, XIANG, BLACK);
    f(1, 9, MA, BLACK); f(7, 9, MA, BLACK);
    f(0, 9, JU, BLACK); f(8, 9, JU, BLACK);
    f(1, 7, PAO, BLACK); f(7, 7, PAO, BLACK);
    f(0, 6, BING, BLACK); f(2, 6, BING, BLACK); f(4, 6, BING, BLACK); f(6, 6, BING, BLACK); f(8, 6, BING, BLACK);
}

std::vector<Pos> possibleMove(int x, int y) {
    static const int dx1[4] = {0, -1, 1, 0}, dy1[4] = {1, 0, 0, -1}; // jiang, bing, ju, pao
    static const int dx2[4] = {1, 1, -1, -1}, dy2[4] = {1, -1, 1, -1}; // shi, xiang * 2
    static const int dx3[8] = {2, 1, -1, -2, -2, -1, 1, 2}, dy3[8] = {1, 2, 2, 1, -1, -2, -2, -1}; // ma
    static const int dx4[8] = {1, 0, 0, -1, -1, 0, 0, 1}, dy4[8] = {0, 1, 1, 0, 0, -1, -1, 0}; // ma jiao
    std::vector<Pos> ans;
    if (type[x][y] == JIANG) {
        for (int k = 0; k < 4; ++k) {
            int fx = x + dx1[k], fy = y + dy1[k];
            if (fx < 3 || fy < 0 || fx > 5 || fy > 9 || color[fx][fy] == color[x][y])
                continue;
            if (color[x][y] == RED && fy > 2)
                continue;
            if (color[x][y] == BLACK && fy < 7)
                continue;
            ans.push_back(Pos(fx, fy));
        }
    } else if (type[x][y] == SHI) {
        for (int k = 0; k < 4; ++k) {
            int fx = x + dx2[k], fy = y + dy2[k];
            if (fx < 3 || fy < 0 || fx > 5 || fy > 9 || color[fx][fy] == color[x][y])
                continue;
            if (color[x][y] == RED && fy > 2)
                continue;
            if (color[x][y] == BLACK && fy < 7)
                continue;
            ans.push_back(Pos(fx, fy));
        }
    } else if (type[x][y] == XIANG) {
        for (int k = 0; k < 4; ++k) {
            int fx = x + dx2[k] * 2, ffx = x + dx2[k], fy = y + dy2[k] * 2, ffy = y + dy2[k];
            if (fx < 0 || fy < 0 || fx > 8 || fy > 9 || color[fx][fy] == color[x][y])
                continue;
            if (color[x][y] == RED && fy > 4)
                continue;
            if (color[x][y] == BLACK && fy < 5)
                continue;
            if (type[ffx][ffy] != NOTYPE)
                continue;
            ans.push_back(Pos(fx, fy));
        }
    } else if (type[x][y] == MA) {
        for (int k = 0; k < 8; ++k) {
            int fx = x + dx3[k], ffx = x + dx4[k], fy = y + dy3[k], ffy = y + dy4[k];
            if (fx < 0 || fy < 0 || fx > 8 || fy > 9 || color[fx][fy] == color[x][y])
                continue;
            if (type[ffx][ffy] != NOTYPE)
                continue;
            ans.push_back(Pos(fx, fy));
        }
    } else if (type[x][y] == JU) {
        for (int k = 0; k < 4; ++k) {
            int fx = x + dx1[k], fy = y + dy1[k];
            while (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9 && type[fx][fy] == NOTYPE) {
                ans.push_back(Pos(fx, fy));
                fx += dx1[k];
                fy += dy1[k];
            }
            if (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9 && color[fx][fy] != color[x][y])
                ans.push_back(Pos(fx, fy));
        }
    } else if (type[x][y] == PAO) {
        for (int k = 0; k < 4; ++k) {
            int fx = x + dx1[k], fy = y + dy1[k];
            while (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9 && type[fx][fy] == NOTYPE) {
                ans.push_back(Pos(fx, fy));
                fx += dx1[k];
                fy += dy1[k];
            }
            if (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9) {
                fx += dx1[k];
                fy += dy1[k];
                while (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9 && type[fx][fy] == NOTYPE) {
                    fx += dx1[k];
                    fy += dy1[k];
                }
                if (fx >= 0 && fy >= 0 && fx <= 8 && fy <= 9 && color[fx][fy] != color[x][y])
                    ans.push_back(Pos(fx, fy));
            }
        }
    } else if (type[x][y] == BING) {
        int g = 1, rev = 1;
        if ((color[x][y] == RED && y > 4) || (color[x][y] == BLACK && y < 5))
            g = 3;
        if (color[x][y] == BLACK)
            rev = -1;
        for (int k = 0; k < g; ++k) {
            int fx = x + dx1[k], fy = y + dy1[k] * rev;
            if (fx < 0 || fy < 0 || fx > 8 || fy > 9 || color[fx][fy] == color[x][y])
                continue;
            ans.push_back(Pos(fx, fy));
        }
    }
    return ans;
}

int moveTo(int x1, int y1, int x2, int y2, Color c) {
    if (color[x1][y1] != c)
        return false;
    for (auto &p : possibleMove(x1, y1)) {
        if (p.x == x2 && p.y == y2) {
            Type tmpT = type[x2][y2];
            Color tmpC = color[x2][y2];
            type[x2][y2] = type[x1][y1];
            color[x2][y2] = c;
            type[x1][y1] = NOTYPE;
            color[x1][y1] = NOCOLOR;
            if (checkDuiJiang() || checkJiang(Color(1 - c))) {
                type[x1][y1] = type[x2][y2];
                color[x1][y1] = color[x2][y2];
                type[x2][y2] = tmpT;
                color[x2][y2] = tmpC;
                return -1;
            }
            return 1;
        }
    }
    return 0;
}

void f(int i, int j, Type t, Color c) {
    type[i][j] = t;
    color[i][j] = c;
}

bool checkWin(Color c) {
    bool exist = false;
    for (int i = 0; i < 9 && !exist; ++i)
        for (int j = 0; j < 10 && !exist; ++j)
            if (type[i][j] == JIANG && color[i][j] == Color(1 - c))
                exist = true;
    if (!exist)
        return true;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j)
            if (color[i][j] == Color(1 - c))
                for (auto &p : possibleMove(i, j)) {
                    int x = p.x, y = p.y;
                    Type tmpT = type[x][y];
                    Color tmpC = color[x][y];
                    type[x][y] = type[i][j];
                    color[x][y] = color[i][j];
                    type[i][j] = NOTYPE;
                    color[i][j] = NOCOLOR;
                    bool lose = false;
                    if (checkDuiJiang() || checkJiang(c))
                        lose = true;
                    type[i][j] = type[x][y];
                    color[i][j] = color[x][y];
                    type[x][y] = tmpT;
                    color[x][y] = tmpC;
                    if (!lose)
                        return false;
                }
    return true;
}

bool checkJiang(Color c) {
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j)
            if (color[i][j] == c)
                for (auto &p : possibleMove(i, j))
                    if (type[p.x][p.y] == JIANG)
                        return true;
    return false;
}

bool checkDuiJiang() {
    Pos p[2];
    p[0].x = p[1].x = -1;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j)
            if (type[i][j] == JIANG)
                p[color[i][j]] = Pos(i, j);
    if (p[0].x == -1 || p[1].x == -1)
        return false;
    if (p[0].x != p[1].x)
        return false;
    for (int i = std::min(p[0].y, p[1].y) + 1; i <= std::max(p[0].y, p[1].y) - 1; ++i)
        if (type[p[0].x][i] != NOTYPE)
            return false;
    return true;
}

}
