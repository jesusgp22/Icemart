#include "addfooddialog.h"
#include "ui_addfooddialog.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>


AddFoodDialog::AddFoodDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFoodDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    if(query.exec("SELECT name, measure_unit FROM Food")){
        int i=0;
        while(query.next()){
            ui->foodComboFox->insertItem(i++,query.value(0).toString(),QVariant(query.value(1).toString()));
        }
    }else{
        qDebug()<<"Couldn't retrieve food from database";
    }
    ui->foodComboFox->setEditable(true);
    ui->dateEdit->setDate(QDate::currentDate());
}

AddFoodDialog::~AddFoodDialog()
{
    delete ui;
}

void AddFoodDialog::on_buttonBox_accepted()
{
    //add to database
    if(ui->amountSpinBox->value()==0){
        return;
    }
    QSqlQuery query;
    int food_id;
    if(query.exec("SELECT id FROM Food WHERE name=\""+ui->foodComboFox->currentText()+"\";")){
        query.next();
        food_id = query.value(0).toInt();
    }else{
        qDebug()<<query.lastError().text();
        return;
    }
    query.prepare("INSERT INTO Item (expiration_date,amount,food_id) VALUES ( :expiration_date, :amount, :food_id)");
    query.bindValue(":expiration_date",ui->dateEdit->date());
    query.bindValue(":amount",ui->amountSpinBox->value());
    query.bindValue(":food_id",food_id);
    if(query.exec()){
        qDebug()<<"Item added successfully";
    }else{
        qDebug()<<"Failed to insert food item into database";
        qDebug()<<query.lastError().text();
    }
}

void AddFoodDialog::on_foodComboFox_currentIndexChanged(int index)
{
    QString unit = ui->foodComboFox->itemData(index).toString();
    ui->unitLabel->setText(unit);
}
