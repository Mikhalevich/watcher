
QT       += core gui network

#plugin name
#should be same as plugin name in .cpp file
TARGET = PluginExample

TEMPLATE = lib
CONFIG += plugin

DESTDIR = ./../plugins

INCLUDEPATH += ./include

LIBS += -lClientTcpSocket -L./../lib

SOURCES += \
    pluginexample.cpp

HEADERS += \
    pluginexample.h

FORMS += \
    pluginexample.ui








