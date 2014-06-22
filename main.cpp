#include "mainwindow.h"
#include <QApplication>
#include "dbConnection.h"
#include<QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("IAB2014JMW");
    a.setApplicationName("Icemart");

    if (!createConnection())
    {
        return 1;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
