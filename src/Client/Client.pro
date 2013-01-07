#-------------------------------------------------
#
# Project created by QtCreator 2012-04-04T15:25:05
#
#-------------------------------------------------

TEMPLATE = app

QT       += core gui network testlib

CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = Client_d
    DESTDIR = ./../bin_d
    DEFINES += DEBUG
    CONFIG += console
} else {
    TARGET = Client
    DESTDIR = ./../bin
}

DEFINES += QT_NO_CAST_FROM_ASCII

INCLUDEPATH += ./../Server \
    ./../ClientTcpSocket \
    ./../Server/helper/
LIBS += -lClientTcpSocket -L./../lib

SOURCES += main.cpp \
    mainwindow.cpp \
    connectdialog.cpp \
    aurorizationdialog.cpp

HEADERS  += \
    mainwindow.h \
    connectdialog.h \
    aurorizationdialog.h

FORMS    += \
    mainwindow.ui \

TRANSLATIONS += ../language/Client_en.ts




















