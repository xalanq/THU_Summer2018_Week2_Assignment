#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <vector>

#include <QSoundEffect>
#include <QTimer>
#include <QWidget>

namespace Ui {
class PanelWidget;
}

/**
 * @brief 窗口上方的部件，用于显示“轮到谁进行下棋”、“当前回合所剩时间”、“认输按钮”
 */
class PanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = 0);
    ~PanelWidget();

    /**
     * @brief 初始化部件的信息
     *
     */
    void init();

    /**
     * @brief 切换当前下棋的选手
     * @param red 若为true，表示当前下棋的是红方；否则是黑方
     */
    void setTurn(bool red);

    /**
     * @brief 设置认输按钮是否可用
     * @param enabled 若为true表示可用；否则表示不可用
     */
    void setRenshuEnabled(bool enabled);

    /**
     * @brief 设置每个回合的上限时间
     * @param limit 上限，时间为秒
     */
    void setTimelimit(int limit);

    /**
     * @brief 获得每个回合的上限时间
     * @return 返回limit
     * @see setTimelimit(int limit)
     */
    int getTimelimit() const;

    Ui::PanelWidget *ui;

signals:
    /**
     * @brief 当认输按钮按下后，发出信号
     */
    void clickedRenshu(bool);

    /**
     * @brief 当倒计时到0后，发出信号
     */
    void timeout();

private:
    QTimer *tm; ///< 倒计时用的定时器
    int sec; ///< 该回合还剩多少秒
    int limit; ///< 每个回合的上限时间

    std::vector<QSoundEffect *> sounds; ///< 倒计时的音效（从10开始倒数）
};

#endif // PANELWIDGET_H
