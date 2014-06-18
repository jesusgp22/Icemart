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
    emit setPage(4);
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
    emit setPage(1);
}

void MainWidget::on_recipeButton_clicked()
{
    emit setPage(2);
}

void MainWidget::on_dietButton_clicked()
{
    emit setPage(3);
}

void MainWidget::on_listButton_clicked()
{
    emit setPage(5);
}
