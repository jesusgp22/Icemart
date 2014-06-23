#ifndef FOODWIDGET_H
#define FOODWIDGET_H

#include <QWidget>

namespace Ui {
class FoodWidget;
}

class FoodWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FoodWidget(QWidget *parent = 0);
    ~FoodWidget();

private slots:

    void on_homeButton_clicked();

    void on_searchButton_clicked();

    void on_addFoodButton_clicked();

    void on_removeFoodButton_clicked();

private:
    Ui::FoodWidget *ui;
    void FillTable();

signals:
    void setPage(int index);
};

#endif // FOODWIDGET_H
