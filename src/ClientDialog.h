#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QRegExp>

#include "LoadingDialog.h"

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QWidget *parent = 0);
    ~ClientDialog();
    QTcpSocket *getSocket() const;

public slots:
    void accept() override;

private:
    Ui::ClientDialog *ui;
    LoadingDialog *loadingDialog;
    QString ip;
    QString port;

    QTcpSocket *socket;
    QRegExp reg;
};

#endif // CLIENTDIALOG_H
