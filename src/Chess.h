#ifndef CHESS_H
#define CHESS_H

#include <vector>

/**
 * 包含中国象棋的操作逻辑的命名空间
 */
namespace Chess {

/**
 * @brief 棋子的种类
 */
enum Type {
    JIANG, ///< 将、帅
    SHI, ///< 仕
    XIANG, ///< 象、相
    MA, ///< 马
    JU, ///< 车
    PAO, ///< 炮
    BING, ///< 并、卒
    NOTYPE ///< 没有棋子
};

/**
 * @brief 两边的颜色
 */
enum Color {
    RED, ///< 红方
    BLACK, ///< 黑方
    NOCOLOR ///< 没有颜色
};

/**
 * @brief 棋盘上的坐标，红方左下角的车为(0, 0)
 */
struct Pos {
    int x, ///< 横坐标
        y; ///< 纵坐标
    Pos(int x = 0, int y = 0) : x(x), y(y) {}
};

/**
 * @brief 将棋盘信息全部清空
 *
 * 即将 #Chess::type 和 #Chess::color 分别置为 #Chess::NOTYPE 和 #Chess::NOCOLOR
 */
extern void clear();

/**
 * @brief 将棋盘置为中国象棋的初始局面
 */
extern void init();

/**
 * @brief 判断颜色为 \p c 一方（刚下完，轮到对方下棋）是否能取得胜利
 * @param c 红方或者黑方
 * @return 若 \p c 能取得胜利，则返回true；否则返回false
 *
 * 这个函数是枚举对方所有可行走法，判断是否存在一种不败的方案。如果没有，则表示 \p c 这一方胜利。
 */
extern bool checkWin(Color c);

/**
 * @brief 判断颜色为 \p c 一方（刚下完，轮到对手下棋）
 * @param c 红方或者黑方
 * @return 若 \p c 将军了，则返回true；否则返回false
 *
 * 枚举 \p c 的所有棋子，判断是否能吃到对面的“将”。
 */
extern bool checkJiang(Color c);

/**
 * @brief 判断两方的“将”、“帅”是否在同一线上，且没有其余棋子在之间。
 * @return 若在一条线上且之间没有棋子，则返回true；否则返回false。
 *
 * 找到两个“将”的位置，枚举他们之间是否有棋子即可判断。
 */
extern bool checkDuiJiang();

/**
 * @brief 求出位置为(x, y)的棋子下一步所有可以走的位置
 * @param x 横坐标
 * @param y 纵坐标
 * @return 返回所有可走的位置。若(x, y)上无棋子，返回一个空的序列。
 *
 * 根据中国象棋的规则，枚举这个位置上的棋子所有可以走的位置
 */
extern std::vector<Pos> possibleMove(int x, int y);

/**
 * @brief 将(x1, y1)位置上的棋子移动到(x2, y2)位置上，且判断 \p c 这一方移动完以后是否“被将”。
 * @param x1 横坐标
 * @param y1 纵坐标
 * @param x2 横坐标
 * @param y2 纵坐标
 * @param c 当前移动方的颜色
 * @return 若(x1, y1)棋子移动到(x2, y2)合法，但“被将”，则不会做此移动，且返回-1；如果合法但没有“被将”，返回1；若不合法，返回0.
 *
 * 调用 \ref possibleMove 获得(x1, y1)所有位置，然后再用 \ref checkJiang 和 \ref checkDuiJiang 判断是否“被将”。
 */
extern int moveTo(int x1, int y1, int x2, int y2, Color c);

/**
 * @brief 将(i, j)位置上的 #Chess::type 和 #Chess::color 相应的赋值成 \p
 * @param i 横坐标
 * @param j 纵坐标
 * @param t 棋子种类
 * @param c 棋子颜色
 */
extern void f(int i, int j, Type t, Color c);

extern Type type[9][10]; ///< type[i][j]表示位置(i, j)上的棋子类型
extern Color color[9][10]; ///< color[i][j]表示位置(i, j)上的棋子颜色
}

#endif // CHESS_H
