#ifndef SUGESTIONWIDGET_H
#define SUGESTIONWIDGET_H

#include <QWidget>

namespace Ui {
class SugestionWidget;
}

class SugestionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SugestionWidget(QWidget *parent = 0);
    ~SugestionWidget();

private:
    Ui::SugestionWidget *ui;

signals:
    void setPage(int);
private slots:
    void on_pushButton_4_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
    void on_checkBox_5_clicked();
    void on_noButton_clicked();
    void on_yesButton_clicked();
    void on_pushButton_clicked();
    void on_homeButton_clicked();
    void on_homeButton_2_clicked();
    void on_stackedWidget_currentChanged(int arg1);
};

#endif // SUGESTIONWIDGET_H
