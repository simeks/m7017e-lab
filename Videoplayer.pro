#-------------------------------------------------
#
# Project created by QtCreator 2013-11-18T21:32:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Videoplayer
TEMPLATE = app


SOURCES += src/main.cpp src/qt/playerwindow.cpp

HEADERS  += src/qt/playerwindow.h

FORMS    += src/qt/playerwindow.ui

RESOURCES += \
    application.qrc
