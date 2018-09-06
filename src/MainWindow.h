#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QSoundEffect>
#include <QTcpSocket>

#include "ChessWidget.h"
#include "ClientDialog.h"
#include "PanelWidget.h"
#include "ServerDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void actionInit();
    void actionCanju();
    void actionSaveCanju();
    void actionClose();
    void actionServer();
    void actionClient();
    void actionRenshu();
    void actionAbout();
    void actionHelp();

    void netRead();

    void netWriteServer();
    void netReadSever(QDataStream &st);

    void netWriteLose();
    void netReadLose(QDataStream &st);

    void netWriteMove(int x1, int y1, int x2, int y2);
    void netReadMove(QDataStream &st);

    void netWriteWin();
    void netReadWin(QDataStream &st);

private:
    static const int MAX_LEN = 1024;
    void setUI();

    Ui::MainWindow *ui;
    ChessWidget *w;

    QSoundEffect *soundStart;
    QSoundEffect *soundWin;
    QSoundEffect *soundLose;

    QTcpSocket *socket;

    ServerDialog *serverDialog;
    ClientDialog *clientDialog;

    PanelWidget *panelWidget;
    QDockWidget *dockWidget;
};

#endif // MAINWINDOW_H
