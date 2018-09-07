#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QSoundEffect>
#include <QTcpSocket>

#include "ChessWidget.h"
#include "ClientDialog.h"
#include "PanelWidget.h"
#include "ServerDialog.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief 主要的窗体，里面包含网络通信
 *
 * ## 网络通信部分
 *
 * 第一个数字表示操作类型：
 *   * 0: server传输初始数据到client
 *   * 1: 移动棋子
 *   * 2: 认输
 *   * 3: 我赢了！
 *
 * 对于0操作：\n
 *    首先传输client是红方(0)还是黑方(1)，接着传输每一回合的时间，最后将残局文件传输过去
 *
 * 对于1操作：\n
 *    接下来4个整数x1, y1, x2, y2，表示坐标(x1, y1)移动到(x2, y2)
 *
 * 对于2操作：\n
 *    不传输任何其他东西
 *
 * 对于3操作：\n
 *    不传输任何其他东西
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * @brief 将当前局面初始化
     */
    void actionInit();

    /**
     * @brief 打开残局文件
     */
    void actionOpenCanju();

    /**
     * @brief 保存当前局面到文件
     */
    void actionSaveCanju();

    /**
     * @brief 关闭窗口
     *
     * 加了一个“是否确定关闭”的对话框
     */
    void actionClose();

    /**
     * @brief 建立服务器
     */
    void actionServer();

    /**
     * @brief 连接到对战
     */
    void actionClient();

    /**
     * @brief 认输
     */
    void actionRenshu();

    /**
     * @brief 关于我
     */
    void actionAbout();

    /**
     * @brief 教程
     */
    void actionHelp();

    /**
     * @brief 和readyRead()连接的槽，用于接受网络传输过来的数据
     */
    void netRead();

    /**
     * @brief 对应于操作0的数据发送
     */
    void netWriteServer();

    /**
     * @brief 对应于操作0的数据接收，并相应进行操作
     * @param st 接受到的数据的数据流
     */
    void netReadSever(QDataStream &st);

    /**
     * @brief 对应于操作1的数据接收
     * @param x1 横坐标
     * @param y1 纵坐标
     * @param x2 横坐标
     * @param y2 纵坐标
     */
    void netWriteMove(int x1, int y1, int x2, int y2);

    /**
     * @brief 对应于操作1的数据接收，并相应进行操作
     * @param st 接受到的数据的数据流
     */
    void netReadMove(QDataStream &st);

    /**
     * @brief 对应于操作2的数据发送
     */
    void netWriteLose();

    /**
     * @brief 对应于操作2的数据接收，并相应进行操作
     * @param st 接受到的数据的数据流
     */
    void netReadLose(QDataStream &st);

    /**
     * @brief 对应于操作3的数据发送
     */
    void netWriteWin();

    /**
     * @brief 对应于操作3的数据接收，并相应进行操作
     * @param st 接受到的数据的数据流
     */
    void netReadWin(QDataStream &st);

private:
    Ui::MainWindow *ui;
    ChessWidget *w; ///< 主部件

    QSoundEffect *soundStart; ///< 播放开始音效
    QSoundEffect *soundWin; ///< 播放胜利音效
    QSoundEffect *soundLose; ///< 播放失败音效

    QTcpSocket *socket; ///< 用于双方通信的socket，从 #clientDialog 或者 #serverDialog 中获得

    ServerDialog *serverDialog; ///< 建立服务器的对话框
    ClientDialog *clientDialog; ///< 连接到对战的对话框

    PanelWidget *panelWidget; ///< 棋盘上方的部件
    QDockWidget *dockWidget; ///< 包含 #panelWidget 的dock
};

#endif // MAINWINDOW_H
