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
    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_homeButton_clicked();

    void on_searchButton_clicked();

private:
    Ui::FoodWidget *ui;

signals:
    void setPage(int index);
};

#endif // FOODWIDGET_H
