#-------------------------------------------------
#
# Project created by QtCreator 2012-05-15T21:25:06
#
#-------------------------------------------------

QT += network

TARGET = ClientTcpSocket
TEMPLATE = lib
CONFIG += staticlib

CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = ClientTcpSocket_d
    DESTDIR = ./../lib_d
} else {
    TARGET = ClientTcpSocket
    DESTDIR = ./../lib
}

INCLUDEPATH += ./../Server/helper/

SOURCES += \
    clienttcpsocket.cpp \
    querry/windowsquerry.cpp \
    querry/picturequerry.cpp \
    querry/getcontrolinfoquerry.cpp \
    querry/controlsquerry.cpp \
    querry/getmailpropertiesquerry.cpp \
    querry/executionreportquerry.cpp \
    querry/getpicturepropertiesquerry.cpp

HEADERS += \
    global.h \
    clienttcpsocket.h \
    abstractwidget.h \
    basewidget.h \
    querry/windowsquerry.h \
    querry/picturequerry.h \
    querry/getcontrolinfoquerry.h \
    querry/controlsquerry.h \
    querry/abstractquerry.h \
    querry/getmailpropertiesquerry.h \
    querry/executionreportquerry.h \
    querry/getpicturepropertiesquerry.h \
    objectfactory.h \































