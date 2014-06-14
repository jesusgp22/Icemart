#include "dietwidget.h"
#include "ui_dietwidget.h"

DietWidget::DietWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DietWidget)
{
    ui->setupUi(this);
}

DietWidget::~DietWidget()
{
    delete ui;
}
