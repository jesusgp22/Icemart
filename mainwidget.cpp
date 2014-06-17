#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QTime>
#include <QDebug>
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    updateTimer();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_sugestionButton_clicked()
{
    //go to suggestion widget

}

void MainWidget::updateTimer()
{
    QString date = QLocale::system().toString(QDate::currentDate(), QLocale::LongFormat);
    QString time = QLocale::system().toString(QTime::currentTime(), QLocale::ShortFormat);
    ui->timeLabel->setText(time);
    ui->dateLabel->setText(date);
}

void MainWidget::on_profileButton_clicked()
{

}

void MainWidget::on_foodButton_clicked()
{

}

void MainWidget::on_recipeButton_clicked()
{

}

void MainWidget::on_dietButton_clicked()
{

}

void MainWidget::on_listButton_clicked()
{

}
