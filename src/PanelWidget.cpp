#include "PanelWidget.h"
#include "ui_PanelWidget.h"

#include <QDebug>

PanelWidget::PanelWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelWidget) {

    ui->setupUi(this);
    connect(ui->btnRenshu, SIGNAL(clicked(bool)), this, SIGNAL(clickedRenshu(bool)));
    ui->lblTurn->setFixedWidth(ui->btnRenshu->width());

    tm = nullptr;

    auto newSound = [this](const QString &url) {
        auto *t = new QSoundEffect(this);
        t->setSource(QUrl(url));
        return t;
    };

    for (int i = 1; i <= 10; ++i)
        sounds.push_back(newSound(QString("qrc:/res/%1.wav").arg(i)));

    init();
}

PanelWidget::~PanelWidget() {
    delete ui;
}

void PanelWidget::init() {
    ui->lblTurn->setText(tr(u8"还未开始"));
    ui->lcdNumber->display(0);
    ui->btnRenshu->setEnabled(false);
    sec = 0;
    limit = 30;

    if (tm != nullptr)
        delete tm;
    tm = new QTimer(this);
    connect(tm, &QTimer::timeout, [this]{
        --sec;
        ui->lcdNumber->display(sec);
        if (1 <= sec && sec <= 10)
            sounds[sec - 1]->play();
        if (sec == 0)
            emit timeout();
    });
}

void PanelWidget::setTurn(bool red) {
    QString s = tr(u8"轮到");
    if (red)
        s += tr(u8"<b><font color='red'>红方</font></b>");
    else
        s += tr(u8"<b><font color='black'>黑方</font></b>");
    ui->lblTurn->setText(s);

    ui->lcdNumber->display(limit);
    sec = limit;
    tm->stop();
    tm->start(1000);
}

void PanelWidget::setRenshuEnabled(bool enabled) {
    ui->btnRenshu->setEnabled(enabled);
}

void PanelWidget::setTimelimit(int limit) {
    this->limit = limit;
}

int PanelWidget::getTimelimit() const {
    return limit;
}
