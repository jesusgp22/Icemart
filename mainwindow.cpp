#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings;
    if(!settings.contains("initialized")){
        qDebug()<<"Run app setup for the first time";
         //open the setup wizzard
        ui->stackedWidget->setCurrentWidget(ui->setupPage);
    }
    //this->showFullScreen();
}

MainWindow::~MainWindow()
{
    delete ui;
}
