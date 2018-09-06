#include "LoadingDialog.h"

#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>

LoadingDialog::LoadingDialog(const QString &title, QWidget *parent) :
    QDialog(parent) {

    lblMovie = new QLabel(this);
    lblText = new QLabel(this);
    lblText->setAlignment(Qt::AlignCenter);
    btnQuit = new QPushButton(tr(u8"关闭"), this);

    gif = new QMovie(":/res/loading.gif");
    lblMovie->setMovie(gif);
    lblMovie->setAlignment(Qt::AlignCenter);

    auto *layout = new QVBoxLayout;
    layout->addWidget(lblMovie);
    layout->addWidget(lblText);
    layout->addWidget(btnQuit);
    setLayout(layout);

    setWindowTitle(title);

    connect(btnQuit, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

LoadingDialog::~LoadingDialog() {
    delete gif;
}

void LoadingDialog::setText(const QString &text) {
    lblText->setText(text);
}

void LoadingDialog::startLoading() {
    gif->start();
}

void LoadingDialog::stopLoading() {
    gif->stop();
}
