#include "recipewidget.h"
#include "ui_recipewidget.h"
#include <QSqlQuery>
#include <QDebug>

RecipeWidget::RecipeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);
    //fill up the recipe screen with a few (random?) recipes
    //sort by those that can be cooked? sort by most liked
    QSqlQuery query;
    query.exec("SELECT id,name,recipe_type FROM Recipe");
    int i=0;
    while(query.next()){
        QListWidgetItem* item = new QListWidgetItem(query.value(1).toString());
        item->setData(Qt::UserRole,query.value(0).toInt());
        ui->listWidget->insertItem(i++,item);
    }
}

RecipeWidget::~RecipeWidget()
{
    delete ui;
}

void RecipeWidget::on_homeButton_clicked()
{
    emit setPage(0);
}

void RecipeWidget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();
    QSqlQuery query;
    if(query.exec("SELECT * FROM Recipe WHERE id="+QString::number(id)+";")){
        query.next();
        ui->stackedWidget->setCurrentIndex(1);
        ui->backButton->setEnabled(true);
        ui->nameLabel->setText(query.value(1).toString()); //name
        ui->preparationText->setText(query.value(3).toString());
        QSqlQuery subquery;
        if(subquery.exec("SELECT Recipe.verbose_name, Recipe.amount "
                         "FROM Ingredients JOIN Recipe WHERE Ingredientes.recipe_id=Recipe.id"
                         "AND Recipe.id="+QString::number(id)+";")){
            while(subquery.next()){
                ui->ingredientText->insertPlainText(subquery.value(0).toString()+subquery.value(1).toString());

            }
        }else{
            qDebug()<<"Couldn't retrieve ingredientes";
        }
    }else{
        qDebug()<<"Couldn't retrieve recipe from database";
    }

}

void RecipeWidget::on_backButton_clicked()
{
    ui->backButton->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(0);
}
