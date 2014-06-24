#include "foodwidget.h"
#include "ui_foodwidget.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "addfooddialog.h"
#include <QDate>
#include <QMessageBox>

FoodWidget::FoodWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FoodWidget)
{
    ui->setupUi(this);
    FillTable();
}

FoodWidget::~FoodWidget()
{
    delete ui;
}

void FoodWidget::on_homeButton_clicked()
{
    emit setPage(0);
}

void FoodWidget::on_searchButton_clicked()
{

}

void FoodWidget::on_addFoodButton_clicked()
{
    AddFoodDialog *dialog = new AddFoodDialog(this);
    int ret_val = dialog->exec();
    if(ret_val){
        FillTable();
    }
    delete dialog;
}

void FoodWidget::FillTable(){
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QSqlQuery query;
    if(query.exec("SELECT Food.name,Item.amount,Food.measure_unit,Item.expiration_date,Item.id FROM Food JOIN Item WHERE Food.id=Item.food_id AND Item.amount>0")){
        int i=0;
        int j=0;
        while(query.next()){
            ui->tableWidget->insertRow(i);
            QTableWidgetItem *item = new QTableWidgetItem(query.value(0).toString());
            ui->tableWidget->setItem(i,j++,item);
            item = new QTableWidgetItem(QString::number(query.value(1).toInt()));
            item->setData(Qt::UserRole,query.value(4).toInt());
            ui->tableWidget->setItem(i,j++,item);
            item = new QTableWidgetItem(query.value(2).toString());
            ui->tableWidget->setItem(i,j++,item);
            item = new QTableWidgetItem(query.value(3).toDate().toString());
            ui->tableWidget->setItem(i,j++,item);
            i++;
            j=0;
        }
    }else{
        qDebug()<<"Couldn't retrieve food from database";
        qDebug()<<query.lastError().text();
    }
}

void FoodWidget::on_removeFoodButton_clicked()
{
    if(!ui->tableWidget->currentIndex().isValid()){
        QMessageBox::critical(this,"Nada seleccionado","Seleccione un item de la lista para restar a la cantidad en existencia",QMessageBox::Ok);
        return;
    }
    QModelIndexList indexList = ui->tableWidget->selectionModel()->selectedRows();
    int row=0;
    foreach (QModelIndex index, indexList) {
       row = index.row();
    }
    QTableWidgetItem * item = ui->tableWidget->item(row,1);
    int amount = item->text().toInt()-ui->spinBox->value();
    QSqlQuery query;
    QSqlQuery auxquery;

    int id = item->data(Qt::UserRole).toInt();

    //register a consumption
    auxquery.prepare("SELECT food_id FROM Item Where id=?;");
    auxquery.addBindValue(id);
    auxquery.exec();
    auxquery.next();
    int food_id = auxquery.value(0).toInt();
    auxquery.prepare("INSERT INTO Consumption (amount,food_id,consumption_date) VALUES (?,?,?);");
    auxquery.addBindValue(ui->spinBox->value());
    auxquery.addBindValue(food_id);
    auxquery.addBindValue(QDate::currentDate());
    auxquery.exec();

    //modify the item
    QString queryText;

    if(amount==0){
        queryText = "DELETE FROM Item WHERE id="+QString::number(id)+";";
    }else{
        queryText = "UPDATE Item SET amount="+ QString::number(amount) +" WHERE id="+QString::number(id)+";";
    }
    if(query.exec(queryText)){
        qDebug()<<"Item updated properly";
        if(amount==0){
            FillTable();
        }else{
            item->setText(QString::number(amount));
        }
    }else{
        qDebug()<<"Failed to update item";
        qDebug()<<query.lastError().text();
    }


}
