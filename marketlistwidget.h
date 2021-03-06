#ifndef MARKETLISTWIDGET_H
#define MARKETLISTWIDGET_H

#include <QWidget>

namespace Ui {
class MarketListWidget;
}

class MarketListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarketListWidget(QWidget *parent = 0);
    ~MarketListWidget();

private:
    Ui::MarketListWidget *ui;
    void FillList();
    void insertItem(int food_id,int amount);

signals:
    void setPage(int index);

private slots:
    void on_homeButton_clicked();
    void calculateMarketList();
    void on_makeListButton_clicked();
    void on_pushButton_2_clicked();
    void on_addFoodButton_clicked();

    void on_addButton_clicked();

};

#endif // MARKETLISTWIDGET_H
