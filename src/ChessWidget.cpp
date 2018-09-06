#include <string>
#include <vector>

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSoundEffect>
#include <QPainter>
#include <QTextCodec>
#include <QTextStream>
#include <QUrl>

#include "ChessWidget.h"

ChessWidget::ChessWidget(QWidget *parent) :
    QWidget(parent) {

    init();

    auto newSound = [this](const QString &url) {
        auto *t = new QSoundEffect(this);
        t->setSource(QUrl(url));
        return t;
    };

    soundMove = newSound("qrc:/res/move.wav");
    soundJiangjun = newSound("qrc:/res/jiangjun.wav");
    soundChi = newSound("qrc:/res/chi.wav");
    soundWarning = newSound("qrc:/res/warning.wav");

    tm = new QTimer(this);
    tm->setInterval(500);
    connect(tm, &QTimer::timeout, [this]{
        selShow = !selShow;
        this->repaint();
    });

    setFixedSize(860, 943);
}

void ChessWidget::init() {
    Chess::init();
    selX = -1;
    selY = -1;
    color = Chess::RED;
    turnColor = Chess::RED;
    selShow = false;
    win = false;
    end = true;
}

void ChessWidget::loadCanjuFromFile(const QString &filename) {
    QFile f(filename);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream st(&f);
    st.setCodec(QTextCodec::codecForName("GB2312"));

    loadCanju(st.readAll());
}

void ChessWidget::loadCanju(const QString &data) {
    std::string str = data.toStdString();
    int pos = 0;

    auto gc = [&] {
        return str[pos++];
    };

    auto gs = [&] {
        char c = gc();
        std::string s;
        for (; c < 'a' || c > 'z'; c = gc());
        for (; c >= 'a' && c <= 'z'; s.push_back(c), c = gc());
        return s;
    };

    auto gn = [&] {
        char c = gc();
        int x = 0;
        for (; c < '0' || c > '9'; c = gc());
        for (; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = gc());
        return x;
    };

    Chess::clear();
    for (int i = 0; i < 2; ++i) {
        Chess::Color c = Chess::NOCOLOR;
        auto s = gs();
        if (s == "red") {
            if (i == 0)
                turnColor = Chess::RED;
            c = Chess::RED;
        } else {
            if (i == 0)
                turnColor = Chess::BLACK;
            c = Chess::BLACK;
        }
        for (int j = 0; j < 7; ++j) {
            int n = gn();
            for (int k = 0; k < n; ++k) {
                int x = gn(), y = gn();
                Chess::type[x][y] = Chess::Type(j);
                Chess::color[x][y] = c;
            }
        }
    }
}

QString ChessWidget::saveCanjuToString() {
    QString f;
    QTextStream st(&f);

    std::vector<Chess::Pos> pos[2][7];
    std::string c[2];
    c[Chess::RED] = "red";
    c[Chess::BLACK] = "black";

    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 10; ++j)
            if (Chess::type[i][j] != Chess::NOTYPE)
                pos[Chess::color[i][j]][Chess::type[i][j]].push_back(Chess::Pos(i, j));

    int o[2] = {Chess::RED, Chess::BLACK};
    if (turnColor == Chess::BLACK)
        o[0] = Chess::BLACK, o[1] = Chess::RED;
    for (int _ = 0; _ < 2; ++_) {
        int k = o[_];
        st << c[k].c_str() << '\n';
        for (int i = 0; i < 7; ++i) {
            st << pos[k][i].size();
            for (int j = 0; j < int(pos[k][i].size()); ++j)
                st << " <" << pos[k][i][j].x << ',' << pos[k][i][j].y << ">";
            st << '\n';
        }
    }
    st.flush();
    return f;
}

void ChessWidget::saveCanju(QString filename) {
    QFile f(filename);
    f.open(QFile::WriteOnly | QFile::Text);
    QTextStream st(&f);
    st << saveCanjuToString();
    st.flush();
    f.close();
}

void ChessWidget::setColor(bool red) {
    if (red)
        color = Chess::RED;
    else
        color = Chess::BLACK;
}

void ChessWidget::setTurnColor(bool red) {
    if (red)
        turnColor = Chess::RED;
    else
        turnColor = Chess::BLACK;
}

void ChessWidget::startGame() {
    end = false;
    repaint();
    emit signalTurn(turnColor == Chess::RED);
}

void ChessWidget::moveQi(int x1, int y1, int x2, int y2, bool self) {
    bool chi = Chess::color[x2][y2] == Chess::Color(1 - turnColor);
    int ret = Chess::moveTo(x1, y1, x2, y2, turnColor);
    if (ret == -1) {
        soundWarning->play();
        QMessageBox::warning(this,
                             tr(u8"无法移动"),
                             tr(u8"移动该步你就输了"));
    } else if (ret == 1) {
        if (Chess::checkJiang(turnColor))
            soundJiangjun->play();
        else if (chi)
            soundChi->play();
        else
            soundMove->play();
        if (self) {
            tm->stop();
            signalMove(x1, y1, x2, y2);
            selX = -1;
            selY = -1;
        }
        if (turnColor == color && Chess::checkWin(color)) {
            turn();
            setWin(true);
            setEnd(true);
            return;
        }
        turn();
        repaint();
    }
}

bool ChessWidget::isRedTurn() const {
    return turnColor == Chess::RED;
}

bool ChessWidget::isRed() const {
    return color == Chess::RED;
}

void ChessWidget::setEnd(bool net) {
    end = true;
    emit signalEnd(net);
}

void ChessWidget::setWin(bool win) {
    this->win = win;
}

bool ChessWidget::isWin() const {
    return win;
}

void ChessWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.save();
    if (color == Chess::BLACK) {
        p.translate(width(), height());
        p.scale(-1, -1);
    }
    p.drawImage(0, 0, QImage(":/res/bg.jpg"));
    p.restore();

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (Chess::type[i][j] != Chess::NOTYPE) {
                int x = i, y = j;
                if (x == selX && y == selY && (!end && !selShow))
                    continue;
                if (color == Chess::RED)
                    y = 9 - j;
                else
                    x = 8 - i;
                QString file = QString(":/res/") +
                               (Chess::color[i][j] == Chess::RED ? "r" : "b") +
                               QString::number(Chess::type[i][j]) + ".png";
                p.drawImage(posX(x) - QI_WIDTH / 2, posY(y) - QI_WIDTH / 2, QImage(file));
            }
        }
    }
}

void ChessWidget::mousePressEvent(QMouseEvent *event) {
    if (end || turnColor != color)
        return;
    if (selX != -1) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 10; ++j) {
                int x = i, y = j;
                if (color == Chess::RED)
                    y = 9 - j;
                else
                    x = 8 - i;
                double lx = posX(x) - QI_WIDTH / 2, rx = lx + QI_WIDTH;
                double ly = posY(y) - QI_WIDTH / 2, ry = ly + QI_WIDTH;
                if (lx <= event->pos().x() && event->pos().x() <= rx &&
                    ly <= event->pos().y() && event->pos().y() <= ry) {
                    if (Chess::color[i][j] == turnColor) {
                        selX = i;
                        selY = j;
                    } else {
                        moveQi(selX, selY, i, j);
                    }
                    return;
                }
            }
        }
    } else {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (Chess::color[i][j] == turnColor) {
                    int x = i, y = j;
                    if (color == Chess::RED)
                        y = 9 - j;
                    else
                        x = 8 - i;
                    double lx = posX(x) - QI_WIDTH / 2, rx = lx + QI_WIDTH;
                    double ly = posY(y) - QI_WIDTH / 2, ry = ly + QI_WIDTH;
                    if (lx <= event->pos().x() && event->pos().x() <= rx &&
                        ly <= event->pos().y() && event->pos().y() <= ry) {
                        selX = i;
                        selY = j;
                        selShow = false;
                        tm->start();
                        repaint();
                        return;
                    }
                }
            }
        }
    }
    selX = -1;
    selY = -1;
}

double ChessWidget::posX(int x) {
    return 67 + x * 90.5;
}

double ChessWidget::posY(int y) {
    return 63 + y * 90.3;
}

void ChessWidget::turn() {
    turnColor = Chess::Color(1 - turnColor);
    emit signalTurn(int(turnColor == Chess::RED));
}
