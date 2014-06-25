#include "addmarketdialog.h"
#include "ui_addmarketdialog.h"

#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

AddMarketDialog::AddMarketDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMarketDialog)
{
    ui->setupUi(this);
    QSqlQuery query;
    if(query.exec("SELECT name, measure_unit FROM Food")){
        int i=0;
        while(query.next()){
            ui->foodComboBox->insertItem(i++,query.value(0).toString(),QVariant(query.value(1).toString()));
        }
    }else{
        qDebug()<<"Couldn't retrieve food from database";
    }
    ui->foodComboBox->setEditable(true);
}

AddMarketDialog::~AddMarketDialog()
{
    delete ui;
}

void AddMarketDialog::on_buttonBox_accepted()
{
    //add to database
    if(ui->spinBox->value()==0){
        return;
    }
    QSqlQuery query;
    int food_id=0;
    query.prepare("SELECT id FROM Food WHERE name=?");
    query.addBindValue(ui->foodComboBox->currentText());
    if(query.exec()){
        query.next();
        food_id = query.value(0).toInt();
    }else{
        qDebug()<<query.lastError().text();
        return;
    }
    query.prepare("SELECT id,amount FROM MarketList WHERE food_id=?");
    query.addBindValue(food_id);
    query.exec();
    if(query.next()){
        int id=query.value(0).toInt();
        int amount=query.value(1).toInt();
        query.prepare("UPDATE MarketList SET amount=? WHERE id=?");
        query.addBindValue(amount+ui->spinBox->value());
        query.addBindValue(id);
        query.exec();
    }else{
        query.prepare("INSERT INTO MarketList (amount,food_id) VALUES (:amount, :food_id)");
        query.bindValue(":amount",ui->spinBox->value());
        query.bindValue(":food_id",food_id);
        if(query.exec()){
            qDebug()<<"Item added successfully";
        }else{
            qDebug()<<"Failed to insert food item into marketlist database";
            qDebug()<<query.lastError().text();
        }
    }
}

void AddMarketDialog::on_foodComboBox_currentIndexChanged(int index)
{
    QString unit = ui->foodComboBox->itemData(index).toString();
    ui->unitLabel->setText(unit);
}
