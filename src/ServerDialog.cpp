#include "ServerDialog.h"
#include "ui_ServerDialog.h"

#include <QIntValidator>
#include <QNetworkInterface>

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog) {

    ui->setupUi(this);

    ui->editPort->setValidator(new QIntValidator(0, 65535, this));
    ui->editPort->setText("23333");

    ui->editSec->setValidator(new QIntValidator(5, 120, this));
    ui->editSec->setText("30");


    for (auto &h : QNetworkInterface::allAddresses())
        if (h != QHostAddress::LocalHost && h.toIPv4Address())
            ui->editIP->setText(h.toString());

    loadingDialog = new LoadingDialog(tr(u8"建立服务器"), this);
    socket = nullptr;

    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, [this]{
        if (socket != nullptr)
            delete socket;
        socket = server->nextPendingConnection();
        server->close();
        loadingDialog->stopLoading();
        loadingDialog->hide();
        QDialog::accept();
    });
    connect(loadingDialog, &LoadingDialog::rejected, [this]{
        server->close();
    });
}

ServerDialog::~ServerDialog() {
    delete ui;
}

QTcpSocket *ServerDialog::getSocket() const {
    return socket;
}

void ServerDialog::setTurn(const QString &turn) {
    ui->lblTurn->setText(turn);
}

int ServerDialog::getTurn() const {
    return ui->cbboxPlay->currentIndex();
}

int ServerDialog::getSecond() const {
    return ui->editSec->text().toInt();
}

void ServerDialog::accept() {
    if (server->isListening())
        server->close();
    auto port = ui->editPort->text();
    loadingDialog->setText(tr(u8"正在监听%1:%2\n等待连接中...").arg(ip).arg(port));
    loadingDialog->startLoading();
    loadingDialog->show();
    server->listen(QHostAddress::AnyIPv4, port.toInt());
}
