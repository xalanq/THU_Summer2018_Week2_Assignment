#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>

#include "LoadingDialog.h"

namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog {
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = 0);
    ~ServerDialog();
    QTcpSocket *getSocket() const;

    void setTurn(const QString &turn);
    int getTurn() const;
    int getSecond() const;

public slots:
    void accept() override;

private:
    Ui::ServerDialog *ui;

    LoadingDialog *loadingDialog;

    QString ip;

    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // SERVERDIALOG_H
