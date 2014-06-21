#-------------------------------------------------
#
# Project created by QtCreator 2014-06-09T11:46:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Icemart
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setupwidget.cpp \
    mainwidget.cpp \
    sugestionwidget.cpp \
    recipewidget.cpp \
    dietwidget.cpp \
    foodwidget.cpp

HEADERS  += mainwindow.h \
    setupwidget.h \
    mainwidget.h \
    sugestionwidget.h \
    recipewidget.h \
    dietwidget.h \
    foodwidget.h \
    dbConnection.h

FORMS    += mainwindow.ui \
    setupwidget.ui \
    mainwidget.ui \
    sugestionwidget.ui \
    recipewidget.ui \
    dietwidget.ui \
    foodwidget.ui

RESOURCES += \
    res.qrc
