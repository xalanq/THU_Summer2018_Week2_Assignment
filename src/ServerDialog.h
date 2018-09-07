#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

#include "LoadingDialog.h"

namespace Ui {
class ServerDialog;
}

/**
 * @brief “建立服务器”的对话框，可以设置监听的端口、当前用户执红or执黑、每回合时间
 */
class ServerDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = 0);
    ~ServerDialog();

    /**
     * @brief 获得该对话框的socket
     * @return 返回连接成功后的QTcpSocket
     */
    QTcpSocket *getSocket() const;

    /**
     * @brief 设置当前局面的先手是谁（以字符串的形式）
     * @param turn 当前局面的先手（字符串）
     */
    void setTurn(const QString &turn);

    /**
     * @brief 获取当前用户想玩的一方
     * @return 若返回0表示红方，返回1表示黑方
     */
    int getTurn() const;

    /**
     * @brief 获取每回合的时间
     * @return 每回合的时间
     */
    int getSecond() const;

public slots:
    /**
     * @brief 重写对话框的accept槽，增加一些功能
     *
     * 增加的功能包括：弹出等待连接的对话框、server的监听
     */
    void accept() override;

private:
    Ui::ServerDialog *ui;

    LoadingDialog *loadingDialog; ///< 等待连接的对话框

    QTcpServer *server; ///< 监听连接的TcpServer
    QTcpSocket *socket; ///< 连接另一台机器的socket
};

#endif // SERVERDIALOG_H
