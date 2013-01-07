#-------------------------------------------------
#
# Project created by QtCreator 2012-05-15T22:01:44
#
#-------------------------------------------------

QT       += core gui network

TARGET = ActionWindow
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ./../plugins

INCLUDEPATH += ./include

LIBS += -lClientTcpSocket -L./../lib

SOURCES += \
    actionwindow.cpp

HEADERS += \
    actionwindow.h

FORMS += \
    actionwindow.ui





