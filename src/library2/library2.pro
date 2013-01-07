#-------------------------------------------------
#
# Project created by QtCreator 2012-04-03T13:43:57
#
#-------------------------------------------------

TEMPLATE = lib

QT       -= gui

CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = library2_d
    DESTDIR = ../lib_d
} else {
    TARGET = library2
    DESTDIR = ../lib
}

DEFINES += LIBRARY2_LIBRARY

SOURCES += library2.cpp

HEADERS += library2.h\
        library2_global.h
