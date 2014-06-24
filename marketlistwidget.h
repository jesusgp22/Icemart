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

signals:
    void setPage(int index);
};

#endif // MARKETLISTWIDGET_H
