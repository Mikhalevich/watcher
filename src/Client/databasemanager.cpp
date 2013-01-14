
#include <stdexcept>

#include <QSqlDatabase>
#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "databasemanager.h"

namespace database
{
    DatabaseManager* DatabaseManager::instance_ = NULL;

    DatabaseManager* DatabaseManager::instance()
    {
        if (instance_ == 0)
        {
            instance_ = new DatabaseManager();
        }

        return instance_;
    }


    class DatabaseManagerImpl
    {
    public:
        QSqlDatabase database_;
    };


    DatabaseManager::DatabaseManager()
        : impl_(new DatabaseManagerImpl())
    {
        impl_->database_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
        impl_->database_.setDatabaseName(QCoreApplication::applicationDirPath() + QDir::separator() + QLatin1String("client.db"));

        if (!impl_->database_.open())
        {
            // throw something
        }

        createDatabase();
    }

    void DatabaseManager::createDatabase()
    {
        QSqlQuery q;

        if (!q.exec(QLatin1String("PRAGMA synchronous = OFF;")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        if (!q.exec(QLatin1String("PRAGMA journal_mode = OFF;")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // creating connections table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS Connections("
            "id INTEGER PRIMARY KEY, "
            "host VARCHAR(255) NOT NULL, "
            "port INTEGER NOT NULL, "
            "user_name VARCHAR(255), "
            "user_pwd BLOB"
            "); ")))
        {
            qDebug() << q.lastError().text();
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // creating plugins table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS Plugins("
            "id INTEGER PRIMARY KEY, "
            "connection_id INTEGER NOT NULL, "
            "plugin_name VARCHAR(255) NOT NULL, "
            "CONSTRAINT PluginsFK FOREIGN KEY (connection_id) REFERENCES Connections(id) ON DELETE CASCADE ON UPDATE CASCADE "
            "); ")))
        {
            qDebug() << q.lastError().text();
            throw std::runtime_error(q.lastError().text().toStdString());
        }
    }

    void DatabaseManager::runQuery(QSqlQuery& sqlQuery, const QString& queryString, const Placeholders& placeholders /* = Placeholders */)
    {
        if (placeholders.isEmpty())
        {
            // simple querry
            if (!sqlQuery.exec(queryString))
            {
                qDebug() << sqlQuery.lastError().text();
                throw std::runtime_error(sqlQuery.lastError().text().toStdString());
            }
        }
        else
        {
            sqlQuery.prepare(queryString);

            for (Placeholders::const_iterator it = placeholders.begin(), end = placeholders.end(); it != end; ++it)
            {
                sqlQuery.bindValue(it.key(), it.value());
            }

            if (!sqlQuery.exec())
            {
                qDebug() << sqlQuery.lastError().text();
                throw std::runtime_error(sqlQuery.lastError().text().toStdString());
            }
        }
    }

    void DatabaseManager::connections(Connections& connectionList)
    {
        QSqlQuery q;

        runQuery(q, QLatin1String("SELECT host, port, user_name, user_pwd FROM Connections;"));

        while (q.next())
        {
            Connection conn;
            conn.host_ = q.value(0).toString();
            conn.port_ = q.value(1).toInt();
            conn.userName_ = q.value(2).toString();
            conn.userPwd_ = q.value(3).toByteArray();

            connectionList.push_back(conn);
        }
    }

    void DatabaseManager::setConnections(const Connections& connectionList)
    {
        QSqlQuery q;

        runQuery(q, QLatin1String("BEGIN TRANSACTION"));

        // remove previous connections
        runQuery(q, QLatin1String("DELETE FROM Connections;"));

        // insert new connections
        for (Connections::const_iterator it = connectionList.begin(), end = connectionList.end(); it != end; ++it)
        {
            Placeholders placeholders;
            placeholders.insert(QLatin1String(":host"), it->host_);
            placeholders.insert(QLatin1String(":port"), it->port_);
            placeholders.insert(QLatin1String(":user_name"), it->userName_);
            placeholders.insert(QLatin1String(":user_pwd"), it->userPwd_);

            runQuery(q, QLatin1String("INSERT INTO Connections(host, port, user_name, user_pwd) VALUES(:host, :port, :user_name, :user_pwd)"), 
                placeholders);
        }

        runQuery(q, QLatin1String("COMMIT TRANSACTION"));
    }

    void DatabaseManager::plugins(Plugins& pluginList)
    {
        QSqlQuery q;

        runQuery(q, QLatin1String("SELECT host, port, user_name, user_pwd, plugin_name FROM Plugins INNER JOIN Connections ON Plugins.connection_id = Connections.id;"));

        while (q.next())
        {
            Connection conn;
            conn.host_ = q.value(0).toString();
            conn.port_ = q.value(1).toInt();
            conn.userName_ = q.value(2).toString();
            conn.userPwd_ = q.value(3).toByteArray();

            Plugin plugin;
            plugin.connection_ = conn;
            plugin.pluginName_ = q.value(4).toString();

            pluginList.push_back(plugin);
        }
    }

    void DatabaseManager::setPlugins(const Plugins& pluginsList)
    {
        QSqlQuery q;

        runQuery(q, QLatin1String("BEGIN TRANSACTION"));

        // remove previous connections
        runQuery(q, QLatin1String("DELETE FROM Plugins;"));

        // insert new connections
        for (Plugins::const_iterator it = pluginsList.begin(), end = pluginsList.end(); it != end; ++it)
        {
            Placeholders placeholders;
            placeholders.insert(QLatin1String(":host"), it->connection_.host_);
            placeholders.insert(QLatin1String(":port"), it->connection_.port_);
            runQuery(q, QLatin1String("SELECT id FROM Connections WHERE host = :host AND port = :port"),
                placeholders);

            int connection_id = -1;

            if (q.first())
            {
                connection_id = q.value(0).toInt();
            }

            if (connection_id != -1)
            {
                placeholders.clear();
                placeholders.insert(QLatin1String(":connection_id"), connection_id);
                placeholders.insert(QLatin1String(":plugin_name"), it->pluginName_);
                runQuery(q, QLatin1String("INSERT INTO Plugins(connection_id, plugin_name) VALUES(:connection_id, :plugin_name);"), 
                    placeholders);
            }
            else
            {
                Q_ASSERT("maybe insert new connection?" && false);
            }
        }

        runQuery(q, QLatin1String("COMMIT TRANSACTION"));
    }
} // database
