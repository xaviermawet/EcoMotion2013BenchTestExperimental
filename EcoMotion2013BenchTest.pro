#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T10:52:22
#
#-------------------------------------------------

QT       += core gui
CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EcoMotion2013BenchTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Plot/Plot.cpp

HEADERS  += MainWindow.hpp \
    Plot/Plot.hpp

FORMS    += MainWindow.ui

RESOURCES += \
    Resources.qrc
