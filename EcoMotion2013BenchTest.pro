#-------------------------------------------------
#
# Project created by QtCreator 2013-03-25T10:52:22
#
#-------------------------------------------------

QT       += core gui
#LIBS += -L/usr/local/lib -lqwt
CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EcoMotion2013BenchTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Plot/Plot.cpp \
    Utils/QCSVParser.cpp \
    Utils/QException.cpp \
    Plot/Zoomer.cpp \
    Megasquirt/MSDataConverter.cpp \
    Megasquirt/MSManager.cpp \
    Megasquirt/MSFileConverterDialog.cpp \
    Megasquirt/MSDataParameterDialog.cpp \
    Megasquirt/MSFileParameterDialog.cpp

HEADERS  += MainWindow.hpp \
    Plot/Plot.hpp \
    Utils/QCSVParser.hpp \
    Utils/QException.hpp \
    Plot/Zoomer.hpp \
    Megasquirt/MSDataConverter.hpp \
    Megasquirt/MSManager.hpp \
    Megasquirt/MSFileConverterDialog.hpp \
    Megasquirt/MSDataParameterDialog.hpp \
    Megasquirt/MSFileParameterDialog.hpp

FORMS    += MainWindow.ui \
    Megasquirt/MSFileConverterDialog.ui \
    Megasquirt/MSDataParameterDialog.ui \
    Megasquirt/MSFileParameterDialog.ui

RESOURCES += \
    Resources.qrc
