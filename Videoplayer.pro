#-------------------------------------------------
#
# Project created by QtCreator 2013-11-18T21:32:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Videoplayer
TEMPLATE = app


SOURCES += src/main.cpp src/qt/playerwindow.cpp \
    src/qt/playlistwindow.cpp

HEADERS  += src/qt/playerwindow.h \
    src/qt/playlistwindow.h

FORMS    += src/qt/playerwindow.ui \
    src/qt/playlistwindow.ui

RESOURCES += data/application.qrc
