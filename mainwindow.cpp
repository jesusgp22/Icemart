#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QDebug>

#define DEBUG false

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sugestionWidget,SIGNAL(setPage(int)),this,SLOT(setPageByIndex(int)));
    connect(ui->mainWidget,SIGNAL(setPage(int)),this,SLOT(setPageByIndex(int)));
    connect(ui->setupWidget,SIGNAL(setPage(int)),this,SLOT(setPageByIndex(int)));
    connect(ui->foodWidget,SIGNAL(setPage(int)),this,SLOT(setPageByIndex(int)));
    connect(ui->recipeWidget,SIGNAL(setPage(int)),this,SLOT(setPageByIndex(int)));
    QSettings settings;
    if(!DEBUG){
        if(!settings.contains("initialized")){
            qDebug()<<"Run app setup for the first time";
            ui->stackedWidget->setCurrentWidget(ui->setupPage);
        }
    }
    this->setStyleSheet("#MainWindow {background-image: url(:/res/background.jpg)}");
    //this->showFullScreen();
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::setPageByIndex(int index){
    ui->stackedWidget->setCurrentIndex(index);
}
