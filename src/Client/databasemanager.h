#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QScopedPointer>

#include "databasetypes.h"

class QSqlQuery;

namespace database
{
    // d pointer
    class DatabaseManagerImpl;

    class DatabaseManager
    {
    public:
        // singleton
        static DatabaseManager* instance();

        // typedefs
        typedef QMap<QString, QVariant> Placeholders;

        // functions to work with database
        void createDatabase();

        void runQuery(QSqlQuery& sqlQuery, const QString& queryString, const Placeholders& placeholders = Placeholders());

        // connections
        void connections(/* out */ Connections& connectionList);
        void setConnections(const Connections& connectionList);

        // plugins
        void plugins(/* out */ Plugins& pluginList);
        void setPlugins(const Plugins& pluginsList);

    private:
        DatabaseManager();

        // no copy
        DatabaseManager(const DatabaseManager&);
        DatabaseManager& operator=(const DatabaseManager&);

        // singleton
        static DatabaseManager *instance_;

        // d pointer
        QScopedPointer<DatabaseManagerImpl> impl_;
    };
} // database

#endif // DATABASEMANAGER_H
