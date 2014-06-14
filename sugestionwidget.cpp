#include "sugestionwidget.h"
#include "ui_sugestionwidget.h"

SugestionWidget::SugestionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SugestionWidget)
{
    ui->setupUi(this);
}

SugestionWidget::~SugestionWidget()
{
    delete ui;
}
