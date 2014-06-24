#include "marketlistwidget.h"
#include "ui_marketlistwidget.h"

MarketListWidget::MarketListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketListWidget)
{
    ui->setupUi(this);
    ui->listBackground->setStyleSheet("#listBackground {background-image: url(:/res/notebook.png);}");
}

MarketListWidget::~MarketListWidget()
{
    delete ui;
}
