#include "marketlistwidget.h"
#include "ui_marketlistwidget.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QHash>

#define BETHA 0.25

MarketListWidget::MarketListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MarketListWidget)
{
    ui->setupUi(this);
    ui->listBackground->setStyleSheet("#listBackground {background-image: url(:/res/notebook.png);}");
    FillList();
}

void MarketListWidget::FillList(){
    QSqlQuery query;
    if(query.exec("SELECT Food.id, Food.name, MarketList.amount,Food.measure_unit FROM MarketList JOIN Food WHERE MarketList.food_id=Food.id")){
        int i=0;
        while(query.next()){
            QString label = query.value(1).toString()+" "+query.value(2).toString()+" "+query.value(3).toString();
            ui->listWidget->insertItem(i++,label);
        }
    }else{
        qDebug()<<"Couldn't retrieve market list"<<query.lastError().text();
    }
}

MarketListWidget::~MarketListWidget()
{
    delete ui;
}

void MarketListWidget::on_homeButton_clicked()
{
    emit setPage(0);
}

void MarketListWidget::calculateMarketList(){
    QSqlQuery query;
    QHash<int,int> table;
    //get all last week's consumptions and add them up
    query.prepare("SELECT Food.id,Consumption.amount,Food.name"
                  " FROM Consumption JOIN Food"
                  " WHERE Consumption.food_id=Food.id"
                  " AND Consumption.consumption_Date >= ?");
    query.addBindValue(QDate::currentDate().addDays(-7));
    if(query.exec()){
        while(query.next()){
            int amount=query.value(1).toInt();
            int food_id=query.value(0).toInt();
            if(!table.contains(food_id)){
                table.insert(food_id,amount);
            }else{
                table[food_id]=table.value(food_id)+amount;
            }
        }
    }else{
        qDebug()<<"Failed to retrieve consumptions"<<query.lastError().text();
    }

    QSqlQuery subquery;
    subquery.prepare("UPDATE Food SET weekly_consumption=? WHERE id=?");
    if(query.exec("SELECT id,weekly_consumption,name,measure_unit FROM Food")){
        ui->listWidget->clear();
        int i=0;
        while(query.next()){
            int id=query.value(0).toInt();
            float weekly_consumption = query.value(1).toFloat();
            float estimate = 0;
            if(!table.contains(id)){
                if(weekly_consumption==0){
                    continue;
                }
                estimate = (1-BETHA)*weekly_consumption;
            }else{
                estimate = (1-BETHA)*weekly_consumption + BETHA*table.value(id);
            }
            if(estimate<0.05){ //we asume this value insignificantly small
                estimate=0;
            }
            subquery.addBindValue(estimate);
            subquery.addBindValue(id);
            if(subquery.exec()){
                qDebug()<<"Updated weekly consumption estimate for"<<query.value(2).toString()<<estimate;
            }else{
                qDebug()<<"Failed to update weekly consumption";
            }
            if(estimate>0.5){
                int int_estimate = qRound(estimate);
                QString label = query.value(2).toString()+" "+QString::number(int_estimate)+" "+query.value(3).toString();
                ui->listWidget->insertItem(i++,label);
                QSqlQuery auxquery;
                auxquery.prepare("INSERT INTO MarketList (amount,food_id) VALUES (?,?);");
                auxquery.addBindValue(int_estimate); qDebug()<<int_estimate;
                auxquery.addBindValue(id); qDebug()<<id;
                if(auxquery.exec()){
                    qDebug()<<"sucessfully added to market list";
                }else{
                    qDebug()<<"failed to add to market list"<<auxquery.lastError().text();
                }
            }
        }
    }else{
        qDebug()<<"Failed to retrieve foods";
    }
    //clear the consumption table after we are done
    if(query.exec("DELETE FROM Consumption")){
        qDebug()<<"consumption table cleared";
    }else{
        qDebug()<<"Failed to update consumption table";
    }



}

void MarketListWidget::on_makeListButton_clicked()
{
    calculateMarketList();
}

void MarketListWidget::on_pushButton_2_clicked()
{
    QSqlQuery query;
    if(query.exec("DELETE FROM MarketList")){
        qDebug()<<"marketlist cleared";
    }else{
        qDebug()<<"Failed to clear market list";
    }


}
