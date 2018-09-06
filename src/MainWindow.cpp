#include <QFileDialog>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    w = new ChessWidget(this);
    setCentralWidget(w);

    serverDialog = new ServerDialog(this);
    serverDialog->setTurn(w->isRedTurn() ? u8"红方" : u8"黑方");

    clientDialog = new ClientDialog(this);

    socket = nullptr;

    panelWidget = new PanelWidget(this);

    dockWidget = new QDockWidget(this);
    dockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
    dockWidget->setWidget(panelWidget);
    dockWidget->setTitleBarWidget(new QWidget(dockWidget));
    addDockWidget(Qt::TopDockWidgetArea, dockWidget);

    auto newSound = [this](const QString &url) {
        auto *t = new QSoundEffect(this);
        t->setSource(QUrl(url));
        return t;
    };

    soundLose = newSound("qrc:/res/lose.wav");
    soundWin = newSound("qrc:/res/win.wav");
    soundStart = newSound("qrc:/res/start.wav");

    connect(ui->actionInit, SIGNAL(triggered(bool)), this, SLOT(actionInit()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout()));
    connect(ui->actionCanju, SIGNAL(triggered(bool)), this, SLOT(actionCanju()));
    connect(ui->actionClient, SIGNAL(triggered(bool)), this, SLOT(actionClient()));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(actionHelp()));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(actionClose()));
    connect(ui->actionRenshu, SIGNAL(triggered(bool)), this, SLOT(actionRenshu()));
    connect(ui->actionSaveCanju, SIGNAL(triggered(bool)), this, SLOT(actionSaveCanju()));
    connect(ui->actionServer, SIGNAL(triggered(bool)), this, SLOT(actionServer()));

    connect(panelWidget, SIGNAL(clickedRenshu(bool)), this, SLOT(actionRenshu()));
    connect(panelWidget, &PanelWidget::timeout, [this]{
        if (w->isRedTurn() == w->isRed()) {
            w->setWin(false);
            w->setEnd(true);
        }
    });
    connect(w, &ChessWidget::signalTurn, panelWidget, &PanelWidget::setTurn);
    connect(w, &ChessWidget::signalEnd, [this](bool net){
        if (net) {
            if (w->isWin())
                netWriteWin();
            else
                netWriteLose();
        }
        ui->actionRenshu->setEnabled(false);
        panelWidget->init();
        socket = nullptr;
        if (w->isWin()) {
            soundWin->play();
            QMessageBox::information(this,
                                     tr(u8"对战结束"),
                                     tr(u8"你赢了！"));
        } else {
            soundLose->play();
            QMessageBox::information(this,
                                     tr(u8"对战结束"),
                                     tr(u8"你输了..."));
        }
    });
    connect(w, &ChessWidget::signalMove, this, &MainWindow::netWriteMove);

    setWindowIcon(QIcon(":/res/icon.ico"));
}

MainWindow::~MainWindow() {

}

void MainWindow::actionInit() {
    if (socket != nullptr) {
        QMessageBox::warning(this,
                             tr(u8"错误"),
                             tr(u8"你正在对战，无法初始化局面"));
        return;
    }
    auto ret = QMessageBox::warning(this,
                                    tr(u8"初始化局面"),
                                    tr(u8"你确定要初始化局面吗？"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
        w->init();
}

void MainWindow::actionCanju() {
    auto filename = QFileDialog::getOpenFileName(this,
                                                 tr(u8"打开残局"),
                                                 "",
                                                 tr(u8"文本文件 (*.txt)"));
    if (!filename.isEmpty())
        w->loadCanjuFromFile(filename);
}

void MainWindow::actionSaveCanju() {
    auto filename = QFileDialog::getSaveFileName(this,
                                                 tr(u8"保存当前局面"),
                                                 "",
                                                 tr(u8"文本文件 (*.txt)"));
    if (!filename.isEmpty())
        w->saveCanju(filename);
}

void MainWindow::actionClose() {
    auto ret = QMessageBox::warning(this,
                                    tr(u8"退出"),
                                    tr(u8"你确定要退出？？"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if (ret == QMessageBox::Yes)
        close();
}

void MainWindow::actionServer() {
    if (serverDialog->exec() == QDialog::Accepted) {
        socket = serverDialog->getSocket();
        w->setColor(serverDialog->getTurn() == 0);
        ui->actionRenshu->setEnabled(true);
        panelWidget->setTimelimit(serverDialog->getSecond());
        panelWidget->setRenshuEnabled(true);
        w->startGame();
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::netRead);
        netWriteServer();
        QMessageBox::information(this,
                                 tr(u8"对战"),
                                 tr(u8"连接成功！你将与IP为%1的用户对战").arg(socket->peerAddress().toString()));
    }
}

void MainWindow::actionClient() {
    if (clientDialog->exec() == QDialog::Accepted) {
        socket = clientDialog->getSocket();
        ui->actionRenshu->setEnabled(true);
        panelWidget->setRenshuEnabled(true);
        connect(socket, &QTcpSocket::readyRead, this, &MainWindow::netRead);
        QMessageBox::information(this,
                                 tr(u8"对战"),
                                 tr(u8"连接成功！你将与IP为%1的用户对战").arg(socket->peerAddress().toString()));
    }
}

void MainWindow::actionRenshu() {
    auto ret = QMessageBox::warning(this,
                                    tr(u8"认输"),
                                    tr(u8"你确定要认输？？"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        w->setWin(false);
        w->setEnd(true);
    }
}

void MainWindow::actionAbout() {
    QMessageBox::about(this,
                       tr(u8"关于"),
                       tr(u8"<center><b>Week2 Assignment</b></center>" \
                          u8"<p>CopyRight &copy; 2018 by <b>xalanq</b>.</p>" \
                          u8"<p>邮箱: xalanq@qq.com</p>" \
                          u8"<p>协议: LGPL v3.0</p>" \
                          u8"<p>博客: <a href='https://blog.xalanq.com'>blog.xalanq.com</a></p>"));
}

void MainWindow::actionHelp() {
    QMessageBox::information(this,
                             tr(u8"帮助"),
                             tr(u8"<p>对局进行时，轮到你走棋的时候，点击你的棋子，看到棋子闪烁后，再点击你想去到的地方。</p>" \
                                u8"<p>软件会判定你的这一步棋是否会“被将”，并且禁止你走这一步棋。</p>" \
                                u8"<p>当局面为必胜态以后，即无论怎么走都会造成对战的结束，软件会自动判定双方胜负。</p>" \
                                u8"<p>以下是菜单栏的一些说明：</p>" \
                                u8"<p><strong>文件</strong></p>" \
                                u8"<ul>" \
                                u8"<li><strong>初始局面</strong>：让当前显示的局面变成最初始的样子。</li>" \
                                u8"<li><strong>打开残局</strong>：让当前显示的局面变成加载的残局。</li>" \
                                u8"<li><strong>保存当前局面</strong>：将当前显示的局面以文本格式保存下来。</li>" \
                                u8"<li><strong>退出</strong>：退出应用程序。</li>" \
                                u8"</ul>" \
                                u8"<p><strong>对战</strong></p>" \
                                u8"<ul>" \
                                u8"<li><strong>建立服务器</strong>：在本机开放某个端口，等待另一台机器连入进来进行对战。注意，该对战会以当前显示的局面开始进行。你可以修改端口、回合时间、双方颜色的信息。</li>" \
                                u8"<li><strong>连接到对战</strong>：输入指定IP地址（IPV4）和相应的端口进行连接对战。</li>" \
                                u8"<li><strong>认输</strong>：对局进行时，可以认输。</li>" \
                                u8"</ul>" \
                                u8"<p><strong>帮助</strong></p>" \
                                u8"<ul>" \
                                u8"<li><strong>关于</strong>：关于我的一些信息。</li>" \
                                u8"<li><strong>教程</strong>：使用本应用的教程。</li>" \
                                u8"</ul>" \
                                u8"<p>以下是棋盘上方部件栏的一些说明：</p>" \
                                u8"<ul>" \
                                u8"<li>最左侧显示的是当前轮到谁进行下棋，若还未对战，会显示“还未开始”。</li>" \
                                u8"<li>中间是读秒区域，会提醒当前回合还有多少剩余时间，<font color='red'><strong>超时判负</strong></font>。在最后10秒会倒计时（声音）。</li>" \
                                u8"<li>右侧的按钮是认输按钮，当正在进行对局时才能使用。</li>" \
                                u8"</ul>"));
}

/**
 * 第一个数字表示操作类型：
 *   0: server传输初始数据到client
 *   1: 移动棋子
 *   2: 认输
 *   3: 我赢了！
 *
 * 对于0操作：
 *    首先传输client是红方(0)还是黑方(1)，接着传输每一回合的时间，最后将残局文件传输过去
 *
 * 对于1操作：
 *    接下来4个整数x1, y1, x2, y2，表示坐标(x1, y1)移动到(x2, y2)
 *
 * 对于2操作：
 *    不传输任何其他东西
 *
 * 对于3操作：
 *    不传输任何其他东西
 */

#define netWriteBegin \
    QByteArray data; \
    QDataStream st(&data, QIODevice::WriteOnly);

#define netWriteEnd \
    socket->write(data); \
    socket->waitForBytesWritten();

void MainWindow::netRead() {
    auto data = socket->readAll();
    QDataStream st(&data, QIODevice::ReadOnly);
    while (!st.atEnd()) {
        int op;
        st >> op;
        if (op == 0)
            netReadSever(st);
        else if (op == 1)
            netReadMove(st);
        else if (op == 2)
            netReadLose(st);
        else if (op == 3)
            netReadWin(st);
    }
}

void MainWindow::netWriteServer() {
    netWriteBegin

    st << 0;
    st << int(w->isRed());
    st << panelWidget->getTimelimit();
    st << w->saveCanjuToString();

    netWriteEnd

    soundStart->play();
}

void MainWindow::netReadSever(QDataStream &st) {
    int color;
    st >> color;

    int limit;
    st >> limit;
    panelWidget->setTimelimit(limit);

    QString canju;
    st >> canju;

    w->setColor(color == 0);
    w->loadCanju(canju);
    soundStart->play();
    w->startGame();
}

void MainWindow::netWriteMove(int x1, int y1, int x2, int y2) {
    netWriteBegin

    st << 1 << x1 << y1 << x2 << y2;

    netWriteEnd
}

void MainWindow::netReadMove(QDataStream &st) {
    int x1, y1, x2, y2;
    st >> x1 >> y1 >> x2 >> y2;
    w->moveQi(x1, y1, x2, y2, false);
}

void MainWindow::netWriteLose() {
    netWriteBegin

    st << 2;

    netWriteEnd
}

void MainWindow::netReadLose(QDataStream &st) {
    w->setWin(true);
    w->setEnd(false);
}

void MainWindow::netWriteWin() {
    netWriteBegin

    st << 3;

    netWriteEnd
}

void MainWindow::netReadWin(QDataStream &st) {
    w->setWin(false);
    w->setEnd(false);
}
