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

    void on_profileButton_clicked();

    void on_foodButton_clicked();

    void on_recipeButton_clicked();

    void on_dietButton_clicked();

    void on_listButton_clicked();

private:
    Ui::MainWidget *ui;
    QTimer* timer;
};

#endif // MAINWIDGET_H
