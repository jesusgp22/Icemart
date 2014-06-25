#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setPageByIndex(int page);


private slots:
    void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::MainWindow *ui;

signals:
    void requestedSugestion();
};

#endif // MAINWINDOW_H
