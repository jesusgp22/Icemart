#include "foodwidget.h"
#include "ui_foodwidget.h"
#include <QFile>
#include <QDebug>
#include <QTextStream>

FoodWidget::FoodWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FoodWidget)
{
    ui->setupUi(this);
    QFile file(":sql/food.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"No se pudo abrir el archivo de alimentos";
    }
    QTextStream in(&file);
    int i = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList tokens = line.split(",");
        ui->listWidget->insertItem(i++,tokens.at(1)+" "+tokens.at(2)+" "+tokens.at(3));
    }
}

FoodWidget::~FoodWidget()
{
    delete ui;
}

void FoodWidget::on_addButton_clicked()
{
    int row = ui->listWidget->currentRow();
    if(row!=-1){
        QStringList tokens = ui->listWidget->item(row)->text().split(" ");
        int a = tokens.at(1).toInt();
        a++;
        QListWidgetItem *item = ui->listWidget->item(row);
        item->setText(tokens.at(0)+" "+QString::number(a)+" "+tokens.at(2));
    }
}

void FoodWidget::on_removeButton_clicked()
{
    int row = ui->listWidget->currentRow();
    if(row!=-1){
        QStringList tokens = ui->listWidget->item(row)->text().split(" ");
        int a = tokens.at(1).toInt();
        if(a==0){
            return;
        }
        a--;
        QListWidgetItem *item = ui->listWidget->item(row);
        item->setText(tokens.at(0)+" "+QString::number(a)+" "+tokens.at(2));
    }
}

void FoodWidget::on_homeButton_clicked()
{
    emit setPage(0);
}

void FoodWidget::on_searchButton_clicked()
{

}
