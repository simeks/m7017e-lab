#-------------------------------------------------
#
# Project created by QtCreator 2014-01-03T16:01:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ip_telephone
TEMPLATE = app


SOURCES += src/ip_telephone/main.cpp src/ip_telephone/qt/mainwindow.cpp \
    src/ip_telephone/qt/call_dialog.cpp \
    src/ip_telephone/client.cpp \
    src/ip_telephone/qt/incoming_call_dialog.cpp \
    src/ip_telephone/qt/active_call_dialog.cpp

HEADERS  += src/ip_telephone/qt/mainwindow.h \
    src/ip_telephone/qt/call_dialog.h \
    src/ip_telephone/client.h \
    src/ip_telephone/qt/incoming_call_dialog.h \
    src/ip_telephone/qt/active_call_dialog.h

FORMS    += src/ip_telephone/qt/mainwindow.ui \
    src/ip_telephone/qt/call_dialog.ui \
    src/ip_telephone/qt/incoming_call_dialog.ui \
    src/ip_telephone/qt/active_call_dialog.ui
