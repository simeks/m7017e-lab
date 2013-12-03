#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T14:32:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Voicechat
TEMPLATE = app


SOURCES += src/voicechat/main.cpp src/voicechat/qt/mainwindow.cpp

HEADERS  += src/voicechat/qt/mainwindow.h

FORMS    += src/voicechat/qt/mainwindow.ui

RESOURCES += \
    data/voicechat/application.qrc
