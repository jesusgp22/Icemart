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
};

#endif // SUGESTIONWIDGET_H
