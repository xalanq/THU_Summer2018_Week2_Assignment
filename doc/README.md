## 文档

文档分为两部分，一部分在代码中，然后通过doxygen生成HTML。解压本目录下的`html.zip`后打开`index.html`即可查看；第二部分在此说明文档内，在这里会介绍一些架构方面的信息。

### 游戏操作逻辑

相关的命名空间有：
`Chess`：这是包含中国象棋的操作逻辑的命名空间

主要操作是`possibleMove(int x, int y)`，通过整个棋盘每个位置上的信息、中国象棋的规则来获得位置(x, y)这个棋子可以移动到的位置。

其余操作有“判断是否将、帅对视`、”判断当前局面是否为必胜、必败态”、“是否将军”等，这些操作均使用最简单的枚举算法。

### UI框架

相关的类有：
`MainWindow`：主窗体
`ChessWidget`：主窗体里的主要部件，用于显示棋盘、操作棋盘，并且会发出相应音效
`PanelWidget`：一个dock部件，置于主部件上方，用于显示“轮到谁进行下棋”、“当前回合所剩时间”、“认输按钮”
`ClientDialog`：连接到对战的对话框，可以设置IP（IPv4）和端口
`ServerDialog`：建立服务器，可以设置监听的端口、当前用户执红or执黑、每回合时间
`LoadingDialog`：一个能播放动画的等待连接的对话框

棋盘的绘制是用`paintEvent`和`QPainter`进行绘制的。

### 网络通信

使用`QTcpSocket`进行传输数据（监听端是使用`QTcpServer`进行监听，并且只返回一个可用的socket）

发送端是阻塞（同步）发送的，接收端是非阻塞（异步）接收的。

我用的是`QDataStream`来操作`QTcpSocket`，这样省去很多麻烦，比如发送、接受数据只需要简单的这样写：

发送端：
```
QByteArray data;
QDataStream st(&data, QIODevice::WriteOnly);
QString str = "hahaha";
int op = 2333;
st << op << str;
socket->write(data);
socket->waitForBytesWritten();
```

接收端：
```
auto data = socket->readAll();
QDataStream st(&data, QIODevice::ReadOnly);
QString str;
int op;
st >> op >> str; // op == 2333, str == "hahaha"
```

通信协议如下：

第1个数字表示操作类型：
  * 0: server传输初始数据到client；首先传输client是红方(0)还是黑方(1)，接着传输每一回合的时间，最后将残局文件传输过去
  * 1: 移动棋子；接下来4个整数x1, y1, x2, y2，表示坐标(x1, y1)移动到(x2, y2)
  * 2: 认输；接下来不传输任何其他东西
  * 3: 我赢了！接下来不传输任何其他东西