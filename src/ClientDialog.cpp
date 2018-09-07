#include "ClientDialog.h"
#include "ui_ClientDialog.h"

#include <QIntValidator>
#include <QMessageBox>

ClientDialog::ClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog) {

    ui->setupUi(this);

    ui->editPort->setValidator(new QIntValidator(0, 65535, this));
    ui->editPort->setText("23333");

    QString num = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    reg = QRegExp("^" + num +
                  "\\." + num +
                  "\\." + num +
                  "\\." + num + "$");
    ui->editIP->setValidator(new QRegExpValidator(reg, this));

    loadingDialog = new LoadingDialog(tr(u8"连接到对战"), this);
    socket = nullptr;

    connect(loadingDialog, &LoadingDialog::rejected, [this]{
        socket->abort();
    });
}

ClientDialog::~ClientDialog() {
    delete ui;
}

QTcpSocket *ClientDialog::getSocket() const {
    return socket;
}

void ClientDialog::accept() {
    auto ip = ui->editIP->text();
    auto port = ui->editPort->text();
    if (ip.indexOf(reg) == -1) {
        QMessageBox::warning(this,
                             tr(u8"错误"),
                             tr(u8"你输入的ip地址有误，请重新输入"));
        return;
    }
    loadingDialog->setText(tr(u8"正在连接%1:%2...").arg(ip, port));
    loadingDialog->startLoading();
    loadingDialog->show();
    if (socket != nullptr)
        delete socket;
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, [this]{
        loadingDialog->stopLoading();
        loadingDialog->hide();
        QDialog::accept();
    });
    socket->connectToHost(ip, port.toInt());
}
