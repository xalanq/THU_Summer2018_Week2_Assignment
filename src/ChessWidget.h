#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QSoundEffect>
#include <QTimer>
#include <QWidget>

#include "Chess.h"

class ChessWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChessWidget(QWidget *parent = nullptr);

    void init();
    void loadCanjuFromFile(const QString &filename);
    void loadCanju(const QString &data);
    QString saveCanjuToString();
    void saveCanju(QString filename);
    void setColor(bool red);
    void setTurnColor(bool red);
    void startGame();
    void moveQi(int x1, int y1, int x2, int y2, bool self = true);
    bool isRedTurn() const;
    bool isRed() const;
    void setEnd(bool net);
    void setWin(bool win);
    bool isWin() const;

signals:
    void signalEnd(bool);
    void signalTurn(int);
    void signalMove(int, int, int, int);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public slots:

private:
    double posX(int x);
    double posY(int y);
    void turn();

    static const int QI_WIDTH = 84;

    Chess::Color color;
    Chess::Color turnColor;
    int selX, selY;
    bool selShow;
    QTimer *tm;
    QSoundEffect *soundMove;
    QSoundEffect *soundJiangjun;
    QSoundEffect *soundChi;
    QSoundEffect *soundWarning;
    bool win;
    bool end;
};

#endif // CHESSWIDGET_H
