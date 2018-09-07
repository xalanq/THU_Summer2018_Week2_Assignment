#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QSoundEffect>
#include <QTimer>
#include <QWidget>

#include "Chess.h"

/**
 * @brief 展示棋盘、操作棋盘的部件，同时发出相应音效
 *
 * 通过 \ref paintEvent 绘制棋盘和棋子\n
 * 通过 \ref mousePressEvent 监听鼠标点击事件，并作出相应操作
 */
class ChessWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChessWidget(QWidget *parent = nullptr);

    /**
     * @brief 初始化内部变量
     */
    void init();

    /**
     * @brief 从文件中加载残局
     * @param filename 文件名
     * @see loadCanju
     * @see saveCanjuToFile
     *
     * 通过QFile读取文件后使用 \ref loadCanju 加载残局
     */
    void loadCanjuFromFile(const QString &filename);

    /**
     * @brief 使用字符串加载残局
     * @param data 残局格式的字符串
     * @see loadCanjuFromFile
     * @see saveCanju
     */
    void loadCanju(const QString &data);

    /**
     * @brief 将当前局面保存为残局格式的字符串
     * @return 格式化后得到的字符串
     * @see loadCanju
     * @see saveCanjuToFile
     */
    QString saveCanju();

    /**
     * @brief 将当前局面保存到文件
     * @param filename 文件名
     * @see loadCanjuFromFile
     * @see saveCanju
     *
     * 通过调用 \ref saveCanju 得到数据，然后再用QFile保存至文件中
     */
    void saveCanjuToFile(QString filename);

    /**
     * @brief 设置当前用户使用的颜色（红方或黑方）
     * @param red 若为true，表示使用红方；否则使用黑方
     * @see isRed()
     */
    void setColor(bool red);

    /**
     * @brief 设置当前局面轮到哪一种颜色下棋（红方或黑方）
     * @param red 若为true，表示轮到红方；否则轮到黑方
     * @see isRedTurn()
     */
    void setTurnColor(bool red);

    /**
     * @brief 开始游戏
     *
     * 这个函数会发出 \ref signalTurn 的信号，且传入的信息是先手的颜色
     */
    void startGame();

    /**
     * @brief 将位置(x1, y1)上的棋子移动到位置(x2, y2)，并作出相应的反馈、发出相应的信号
     * @param x1 横坐标
     * @param y1 纵坐标
     * @param x2 横坐标
     * @param y2 纵坐标
     * @param self 若为true，则表示是当前用户下的这一步棋，进行相应的反馈
     *
     * 通过调用 \ref Chess::moveTo ，根据其返回值以及self进行相应的提示（音效、对话框等）和相关信息的更新（切换下棋的选手、判断当前局面是否结束等）。
     */
    void moveQi(int x1, int y1, int x2, int y2, bool self = true);

    /**
     * @brief 判断是否轮到红方下棋
     * @return 若当前轮到红方下棋，则返回true；否则返回false
     * @see setTurnColor(bool red)
     */
    bool isRedTurn() const;

    /**
     * @brief 判断当前用户是否为红方
     * @return 若当前用户为红方，则返回true；否则返回false
     * @see setColor(bool red)
     */
    bool isRed() const;

    /**
     * @brief 设置游戏结束，同时选择是否发网络信号（谁赢谁输的信息）给对面
     * @param net 若为true，表示发送网络信号给对方
     */
    void setEnd(bool net);

    /**
     * @brief 设置当前用户是否胜利
     * @param win 若为true，表示当前用户胜利；否则当前用户输了
     * @see isWin()
     */
    void setWin(bool win);

    /**
     * @brief 判断当前用户是否胜利
     * @return 若为true，表示当前用户胜利；否则当前用户输了
     * @see setWin(bool win)
     */
    bool isWin() const;

signals:
    /**
     * @brief 若游戏结束，则发送结束的信号，同时携带 \ref setEnd 中的net
     * @see setEnd()
     */
    void signalEnd(bool);

    /**
     * @brief 换边时发送信号，携带的参数是表示轮到的是否为红方（为红方则为true）
     */
    void signalTurn(bool);

    /**
     * @brief 移动棋子后发送信号，携带的参数是所移动棋子的前后位置
     */
    void signalMove(int, int, int, int);

protected:
    /**
     * @brief 根据信息用QPainter画出棋盘和棋子
     * @param event 事件
     *
     * 根据 #color 判断棋盘是否转置 \n
     * 根据 #selX , #selY , #selShow 判断一个棋子是否要画出（实现闪烁效果）
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief 鼠标按下后，根据坐标判断接下来要进行的行为
     * @param event 事件
     *
     * 首先，枚举每一个棋子，计算出他们所在的部件坐标系上的矩阵，然后判断 \p event 的坐标落在哪一个棋子的矩阵中，从而定位到棋子。
     *
     * 之后，若是空地或者对方的棋子，则调用 \ref moveQi 来尝试移动棋子；若是己方棋子，则更新 #selX , #selY , #selShow 。
     */
    void mousePressEvent(QMouseEvent *event) override;

private:
    /**
     * @brief 根据棋盘的横坐标，计算出其在部件坐标系上的横坐标
     * @param x 棋盘的横坐标
     * @return 返回部件坐标系上的横坐标
     * @see posY(int y)
     */
    double posX(int x);

    /**
     * @brief 根据棋盘的纵坐标，计算出其在部件坐标系上的纵坐标
     * @param y 棋盘的纵坐标
     * @return 返回部件坐标系上的纵坐标
     * @see posX(int x)
     */
    double posY(int y);

    /**
     * @brief 换边，并发出 \ref signalTurn 的信号
     */
    void turn();

    static const int QI_WIDTH = 84; ///< 一个棋子的宽度（像素）

    Chess::Color color; ///< 当前用户所在一方的颜色
    Chess::Color turnColor; ///< 当前局面轮到的一方的颜色
    int selX, ///< 鼠标选中棋子横坐标
        selY; ///< 鼠标选中棋子纵坐标
    bool selShow; ///< 为true时，鼠标选中的棋子显示；否则不显示
    QTimer *tm; ///< 500ms的一个定时器，用于实现棋子闪烁
    QSoundEffect *soundMove; ///< 播放移动棋子的音效
    QSoundEffect *soundJiangjun; ///< 播放将军的音效
    QSoundEffect *soundChi; ///< 播放吃子的音效
    QSoundEffect *soundWarning; ///< 播放警告的音效
    bool win; ///< 若为true表示当前用户赢了；否则表示输了
    bool end; ///< 若为true表示当前并没有在战斗；否则表示正在战斗
};

#endif // CHESSWIDGET_H
