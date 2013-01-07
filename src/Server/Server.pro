#-------------------------------------------------
#
# Project created by QtCreator 2012-04-03T13:24:00
#
#-------------------------------------------------

TEMPLATE = app

#QT -= gui
QT       += gui core network sql
CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = Server_d
    DESTDIR = ./../bin_d
    DEFINES += DEBUG
    CONFIG += console
} else {
    TARGET = Server
    DESTDIR = ./../bin
}

INCLUDEPATH += ./../library2 \
    ./../ClientTcpSocket \
#user depended for python
    c:/Python27/include/ \

LIBS += -L../lib -llibrary2 \
#user depended for python
    -L"c:/Python27/libs/" -lpython27

DEPENDPATH += ../lib


SOURCES += main.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    querry/picturequerry.cpp \
    querry/processquerry.cpp \
    querry/windowsquerry.cpp \
    querry/controlsquerry.cpp \
    querry/setcontroltextquerry.cpp \
    querry/getcontrolinfoquerry.cpp \
    databasemanager.cpp \
    querry/allpicturesquerry.cpp \
    querrythread.cpp \
    querry/currentdatepicturesquerry.cpp \
    querry/periodpicturesquerry.cpp \
    picturetimer.cpp \
    querry/autorizationquerry.cpp \
    querry/mailpropertiesquerry.cpp \
    querry/getmailpropertiesquerry.cpp \
    querry/getpicturepropertiesquerry.cpp \
    querry/timedpicturesquerry.cpp \
    gui/traysettings.cpp \
    gui/usersettings.cpp \
    gui/adduserdialog.cpp \
    gui/settingsdialog.cpp \
    gui/commonsettingsdialog.cpp \
    querry/settingsquerry.cpp \
    querry/userquerry.cpp \
    commonfunctions.cpp \
    gui/autorizationdialog.cpp

HEADERS  += \
    tcpserver.h \
    tcpsocket.h \
    querry/abstractquerry.h \
    querry/picturequerry.h \
    querry/processquerry.h \
    querry/windowsquerry.h \
    querry/controlsquerry.h \
    querry/setcontroltextquerry.h \
    querry/getcontrolinfoquerry.h \
    picturesingleton.h \
    socketthread.h \
    databasemanager.h \
    databasetypes.h \
    querry/allpicturesquerry.h \
    querry/abstractdatabasequerry.h \
    querrythread.h \
    querry/currentdatepicturesquerry.h \
    querry/periodpicturesquerry.h \
    picturetimer.h \
    singletonthread.h \
    querry/autorizationquerry.h \
    querry/mailpropertiesquerry.h \
    querry/getmailpropertiesquerry.h \
    querry/getpicturepropertiesquerry.h \
    querry/timedpicturesquerry.h \
    gui/traysettings.h \
    gui/usersettings.h \
    gui/adduserdialog.h \
    gui/settingsdialog.h \
    gui/commonsettingsdialog.h \
    querry/settingsquerry.h \
    querry/userquerry.h \
    commonfunctions.h \
    gui/autorizationdialog.h \
    helper/objectfactory.h \

FORMS += \
    gui/commonsettingsdialog.ui





























































































