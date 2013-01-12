#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QScopedPointer>

#include "databasetypes.h"

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

        void connections(/* out */ Connections& connectionList);
        void setConnections(const Connections& connectionList);

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
