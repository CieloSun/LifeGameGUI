#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T10:32:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LifeGameGuiTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp

HEADERS  += \
    targetver.h \
    cell.h \
    mainwindow.h

FORMS    += mainwindow.ui

CONFIG   +=c++11
