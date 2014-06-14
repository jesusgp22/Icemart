#ifndef DIETWIDGET_H
#define DIETWIDGET_H

#include <QWidget>

namespace Ui {
class DietWidget;
}

class DietWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DietWidget(QWidget *parent = 0);
    ~DietWidget();

private:
    Ui::DietWidget *ui;
};

#endif // DIETWIDGET_H
