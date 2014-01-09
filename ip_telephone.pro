#-------------------------------------------------
#
# Project created by QtCreator 2014-01-03T16:01:47
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ip_telephone
TEMPLATE = app


SOURCES += src/ip_telephone/main.cpp src/ip_telephone/qt/mainwindow.cpp \
    src/ip_telephone/client.cpp \
    src/ip_telephone/qt/settings_dialog.cpp

HEADERS  += src/ip_telephone/qt/mainwindow.h \
    src/ip_telephone/client.h \
    src/ip_telephone/qt/settings_dialog.h

FORMS    += src/ip_telephone/qt/mainwindow.ui \
    src/ip_telephone/qt/settings_dialog.ui

RESOURCES += \
    data/ip_telephone/application.qrc
