#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void on_sugestionButton_clicked();
    void updateTimer();

private:
    Ui::MainWidget *ui;
    QTimer* timer;
};

#endif // MAINWIDGET_H
