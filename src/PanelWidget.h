#ifndef PANELWIDGET_H
#define PANELWIDGET_H

#include <vector>

#include <QSoundEffect>
#include <QTimer>
#include <QWidget>

namespace Ui {
class PanelWidget;
}

class PanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit PanelWidget(QWidget *parent = 0);
    ~PanelWidget();

    void init();
    void setTurn(bool red);
    void setRenshuEnabled(bool enabled);
    void setTimelimit(int limit);
    int getTimelimit() const;

    Ui::PanelWidget *ui;

signals:
    void clickedRenshu(bool);
    void timeout();

private:
    QTimer *tm;
    int sec;
    int limit;

    std::vector<QSoundEffect *> sounds;
};

#endif // PANELWIDGET_H
