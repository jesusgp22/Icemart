#include "recipewidget.h"
#include "ui_recipewidget.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

RecipeWidget::RecipeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecipeWidget)
{
    ui->setupUi(this);
    //show all recipes
    fillRecipeList();
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
        ui->typeLabel->setText(query.value(2).toString()); //type
        ui->preparationText->setText(query.value(3).toString());
        QSqlQuery subquery;
        QString subquerytext = "SELECT Ingredient.amount, Ingredient.verbose_name, Ingredient.food_id "
                         "FROM Ingredient JOIN Recipe WHERE Ingredient.recipe_id=Recipe.id "
                         "AND Recipe.id="+QString::number(id)+";";
        bool canCook=true;
        if(subquery.exec(subquerytext)){
            ui->ingredientText->clear();
            while(subquery.next()){
                ui->ingredientText->insertPlainText(subquery.value(0).toString()+" "+subquery.value(1).toString());
                //check if there is enough of this on the fridge and add the right icon
                QSqlQuery subsubquery;
                subsubquery.prepare("SELECT amount FROM Item WHERE food_id=?");
                subsubquery.addBindValue(subquery.value(2).toInt());
                if(subsubquery.exec()){
                    int amount = 0;
                    while(subsubquery.next()){
                        amount+=subsubquery.value(0).toInt();
                    }
                    if(amount>=subquery.value(0).toInt()){
                        ui->ingredientText->insertHtml("<img src=\":/res/checkicon.png\" height=\"20\" width=\"20\"><br>");
                    }else{
                        canCook=false;
                        ui->ingredientText->insertHtml("<img src=\":/res/noicon.png\" height=\"20\" width=\"20\"><br>");
                    }
                }else{
                    qDebug()<<subsubquery.lastError().text();
                    ui->ingredientText->insertPlainText("\n");
                }
            }
            subquery.prepare("SELECT VerboseIngredient.verbose_name FROM VerboseIngredient JOIN Recipe WHERE VerboseIngredient.recipe_id=Recipe.id AND Recipe.id=?");
            subquery.addBindValue(id);
            if(subquery.exec()){
                while(subquery.next()){
                    ui->ingredientText->insertPlainText(subquery.value(0).toString());
                    //set ok icon
                    ui->ingredientText->insertHtml("<img src=\":/res/checkicon.png\" height=\"20\" width=\"20\"><br>");

                }
            }
        if(canCook){
            ui->statusLabel->setText("Todos los ingredientes disponibles!");
            ui->optionButton->setIcon(QIcon(":/res/cookicon.png"));
            ui->optionButton->setText("Preparar esta receta");
        }else{
            ui->statusLabel->setText("Faltan ingredientes!");
            ui->optionButton->setIcon(QIcon(":/res/listrecipeicon.png"));
            ui->optionButton->setText("Agregar a la lista de compras");
        }
        }else{
            qDebug()<<"Couldn't retrieve ingredientes";
            qDebug()<<subquery.lastError().text();
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

void RecipeWidget::fillRecipeList(QString filter){
    QSqlQuery query;
    if(filter==""){
        query.exec("SELECT id,name FROM Recipe;");
    }else{
        query.exec("SELECT id,name FROM Recipe WHERE recipe_type=\""+filter+"\";");
    }
    int i=0;
    ui->listWidget->clear();
    while(query.next()){
        QListWidgetItem* item = new QListWidgetItem(query.value(1).toString());
        item->setData(Qt::UserRole,query.value(0).toInt());
        ui->listWidget->insertItem(i++,item);
    }
}

void RecipeWidget::on_soupButton_clicked()
{
    fillRecipeList("Sopas y cremas");
}

void RecipeWidget::on_allIcon_clicked()
{
    fillRecipeList();
}

void RecipeWidget::on_breakfastButton_clicked()
{
    fillRecipeList("Entradas y huevos");
}

void RecipeWidget::on_fishButton_clicked()
{
    fillRecipeList("Pescados y mariscos");
}

void RecipeWidget::on_meatButton_clicked()
{
    fillRecipeList("Carnes");
}

void RecipeWidget::on_chickenButton_clicked()
{
    fillRecipeList("Aves y caza");
}

void RecipeWidget::on_saladButton_clicked()
{
    fillRecipeList("Ensaladas y verduras");
}

void RecipeWidget::on_riceButton_clicked()
{
    fillRecipeList("Pastas y arroces");
}

void RecipeWidget::on_searchButton_clicked()
{
    //tokenize and flattern search query
    QString userQuery(this->ui->searchLine->text());
    if(userQuery.isEmpty()){
        return;
    }
    QStringList rawTokens = userQuery.split(" ");
    QStringList tokens;
    for(int j=0;j<rawTokens.size();j++){
        tokens << rawTokens.at(j).toLower();
    }

    //get all recipes
    QSqlQuery query;
    if(!query.exec("SELECT id,name FROM Recipe")){
        qDebug()<<"Failed to retrieve recipes";
        return;
    }
    int j=0;
    bool found=false;
    ui->listWidget->clear();
    while(query.next()){
        QString name = query.value(1).toString();
        name = name.toLower();
        qDebug()<<name;
        for(int i=0;i<tokens.size();i++){
            if(name.contains(tokens.at(i)))
                found=true;
        }
        if(found){
            found=false;
            QListWidgetItem* item = new QListWidgetItem(query.value(1).toString());
            item->setData(Qt::UserRole,query.value(0).toInt());
            ui->listWidget->insertItem(j++,item);
        }
    }
    if(j==0){
        QMessageBox message(this);
        message.setWindowTitle("No se encontraron recetas");
        message.setText("No se encontraron recetas que coincidan con su busqueda, intenta usar palabras mas generales y revisar la ortografia de las palabras");
        message.setStandardButtons(QMessageBox::Ok);
        message.exec();
    }
}
