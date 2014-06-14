#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("IAB2014JMW");
    a.setApplicationName("Icemart");
    MainWindow w;
    w.show();

    return a.exec();
}
