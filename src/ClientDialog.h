#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QRegExp>

#include "LoadingDialog.h"

namespace Ui {
class ClientDialog;
}

/**
 * @brief “连接到对战”的对话框，可以设置IP（IPv4）和端口
 */
class ClientDialog : public QDialog {
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();

    /**
     * @brief 获得该对话框的socket
     * @return 返回连接成功后的QTcpSocket
     */
    QTcpSocket *getSocket() const;

public slots:
    /**
     * @brief 重写对话框的accept槽，增加一些功能
     *
     * 增加的功能包括：弹出等待连接的对话框、socket的连接
     */
    void accept() override;

private:
    Ui::ClientDialog *ui;
    LoadingDialog *loadingDialog; ///< 等待连接的对话框

    QTcpSocket *socket; ///< 连接另一台机器的socket
    QRegExp reg; ///< 验证ip是否合法的正则表达式
};

#endif // CLIENTDIALOG_H
