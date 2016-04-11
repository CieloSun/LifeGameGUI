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
    cell.cpp \
    cellMap.cpp \
    thread.cpp \
    enddialog.cpp \
    myrestartdialog.cpp \
    settingdialog.cpp \
    aboutusdialog.cpp

HEADERS  += \
    targetver.h \
    cell.h \
    mainwindow.h \
    cellMap.h \
    thread.h \
    enddialog.h \
    myrestartdialog.h \
    settingdialog.h \
    aboutusdialog.h

FORMS    += mainwindow.ui \
    enddialog.ui \
    myrestartdialog.ui \
    settingdialog.ui \
    aboutusdialog.ui

CONFIG   +=c++11

RESOURCES += \
    image.qrc

DISTFILES +=
