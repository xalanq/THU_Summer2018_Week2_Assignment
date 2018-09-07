#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

/**
 * @brief 等待连接的对话框
 */
class LoadingDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoadingDialog(const QString &title, QWidget *parent = nullptr);
    ~LoadingDialog();

    /**
     * @brief 设置等待对话框里的文字信息
     * @param text 要设置的字符串
     */
    void setText(const QString &text);

    /**
     * @brief 开始播放动画
     */
    void startLoading();

    /**
     * @brief 关闭动画
     */
    void stopLoading();

private:
    QLabel *lblMovie; ///< 显示动画的label
    QLabel *lblText; ///< 显示文字的label
    QPushButton *btnQuit; ///< 退出按钮
    QMovie *gif; ///< 动画
};

#endif // LOADINGDIALOG_H
