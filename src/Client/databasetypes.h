#ifndef DATABASETYPES_H
#define DATABASETYPES_H

#include <QList>

namespace database
{
    struct Connection
    {
        QString host_;
        int port_;
        QString userName_;
        QByteArray userPwd_;
    };

    typedef QList<Connection> Connections;

    struct Plugin
    {
        Connection connection_;
        QString pluginName_;
    };

    typedef QList<Plugin> Plugins;
}

#endif // DATABASETYPES_H
