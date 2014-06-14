#include "recipewidget.h"
#include "ui_recipewidget.h"

RecipeWidget::RecipeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);
}

RecipeWidget::~RecipeWidget()
{
    delete ui;
}
