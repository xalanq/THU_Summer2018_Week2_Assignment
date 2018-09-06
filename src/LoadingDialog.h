#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class LoadingDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoadingDialog(const QString &title, QWidget *parent = nullptr);
    ~LoadingDialog();

    void setText(const QString &text);
    void startLoading();
    void stopLoading();

signals:

public slots:

private:
    QLabel *lblMovie;
    QLabel *lblText;
    QPushButton *btnQuit;
    QMovie *gif;
};

#endif // LOADINGDIALOG_H
