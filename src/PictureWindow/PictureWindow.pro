
QT       += core gui network testlib

#plugin name
#should be same as plugin name in .cpp file
TARGET = PictureWindow

TEMPLATE = lib
CONFIG += plugin

CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = PictureWindow_d
} else {
    TARGET = PictureWindow
}

# debug and realease
DESTDIR = ./../plugins

INCLUDEPATH += "./../ClientTcpSocket/" \
    "./../Server/helper/"

LIBS += -lClientTcpSocket -L./../lib

SOURCES += \
    picturewindow.cpp \
    pictureswidget.cpp \
    propertydialog.cpp

HEADERS += \
    picturewindow.h \
    pictureswidget.h \
    propertydialog.h

FORMS += \
    picturewindow.ui \
    propertydialog.ui
















