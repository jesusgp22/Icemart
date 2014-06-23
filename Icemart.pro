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
QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += . \
    /opt/aleph/include
LIBS += -L/opt/aleph/lib \
    -lAleph \
    -lnana \
    -lc \
    -lm \
    -lgsl \
    -lgslcblas \
    -lgmp \
    -lmpfr \
    -lasprintf \
    -lpthread

SOURCES += main.cpp\
        mainwindow.cpp \
    setupwidget.cpp \
    mainwidget.cpp \
    sugestionwidget.cpp \
    recipewidget.cpp \
    dietwidget.cpp \
    foodwidget.cpp \
    addfooddialog.cpp

HEADERS  += mainwindow.h \
    setupwidget.h \
    mainwidget.h \
    sugestionwidget.h \
    recipewidget.h \
    dietwidget.h \
    foodwidget.h \
    Grafo.h \
    dbConnection.h \
    addfooddialog.h

FORMS    += mainwindow.ui \
    setupwidget.ui \
    mainwidget.ui \
    sugestionwidget.ui \
    recipewidget.ui \
    dietwidget.ui \
    foodwidget.ui \
    addfooddialog.ui

RESOURCES += \
    res.qrc
