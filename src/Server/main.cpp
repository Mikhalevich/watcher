
#include <iostream>

#include <QCoreApplication>
#include <QDebug>
#include <QApplication>
#include <QMetaType>

#include "network/tcpserver.h"
#include "commonfunctions.h"
#include "picturesingleton.h"
#include "databasetypes.h"
#include "gui/traysettings.h"
#include "database/query/settingsquery.h"
#include "clipboardsingleton.h"

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);
    QApplication app(argc, argv);
    app.setApplicationName(QLatin1String("watcher"));
    app.setApplicationVersion(QLatin1String("1.0"));
    app.setQuitOnLastWindowClosed(false);

    // create main threads
    SingletonThread<database::DatabaseManager> databaseThread;
    databaseThread.start();
    // wait while database thread initialize
    databaseThread.waitWhileInitialize();

    SingletonThread<PictureTimer> pictureTimerThread;
    pictureTimerThread.start();
    pictureTimerThread.waitWhileInitialize();

    // create get picture object in main thread
    PictureSingleton::instance();

    // create clipboard thread
    SingletonThread<ClipboardSingleton> clipboardThread;
    clipboardThread.start();
    clipboardThread.waitWhileInitialize();

    // get setttings from database
    database::databasequery::GetSettingsQuery settingsQuerry;
    settingsQuerry.execute();

    // parse conmmand line parameters
    int port = settingsQuerry.port();
    parseCommandLine(port);

    // autorun parameters
    checkAutorunSettings(settingsQuerry.startupMode());

    // start server
    network::TcpServer server;
    if (!server.listen(QHostAddress::Any, port))
    {
        qDebug() << server.errorString();
        return 1;
    }
    else
    {
        qDebug() << QString("Server start at %1 port...").arg(port);
    }

    // start gui settings if need
    QSharedPointer<TraySettings> tray;
    if (settingsQuerry.trayIcon())
    {
        tray = QSharedPointer<TraySettings>(new TraySettings());
        tray->show();
    }

    int retCode = app.exec();

    databaseThread.exit();
    pictureTimerThread.exit();
    clipboardThread.exit();

    databaseThread.wait();
    pictureTimerThread.wait();
    clipboardThread.wait();

    return retCode;
}
