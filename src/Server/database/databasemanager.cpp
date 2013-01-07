
#include <stdexcept>

#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QBuffer>
#include <QDebug>
#include <QCoreApplication>

#include "databasemanager.h"

namespace database
{
    DatabaseManager::DatabaseManager(QObject *parent)
        : QObject(parent)
    {
        database_ = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"));
        // full path
        database_.setDatabaseName(QCoreApplication::applicationDirPath() + QLatin1String("/main.db"));

        database_.open();

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

        // creating users table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS Users("
                                  "id INTEGER PRIMARY KEY, "
                                  "user_name VARCHAR(255) NOT NULL, "
                                  "user_pwd BLOB NOT NULL, "
                                  "CONSTRAINT UsersUnique UNIQUE (user_name) "
                                  "); ")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // creating programm settings table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS Settings("
                                  "id INTEGER PRIMARY KEY, "
                                  "port INTEGER DEFAULT 7777, "
                                  "tray_icon INTEGER DEFAULT 0, "
                                  "startup_mode INTEGER DEFAULT 0 "
                                  "); ")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // creating picture table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS Pictures( "
                                 "id INTEGER PRIMARY KEY, "
                                 "picture BLOB, "
                                 "date VARCHAR(255) DEFAULT (strftime('%Y-%m-%d %H:%M:%f', 'now', 'localtime')), "
                                 "state INTEGER DEFAULT 0 "
                                 "); ")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // crating mail properties table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS MailProperties("
                                  "id INTEGER PRIMARY KEY, "
                                  "server VARCHAR(255) NOT NULL, "
                                  "port INTEGER NOT NULL, "
                                  "user VARCHAR(255) NOT NULL, "
                                  "password VARCHAR(255) NOT NULL, "
                                  "send_from VARCHAR(255) NOT NULL, "
                                  "send_to VARCHAR(255) NOT NULL, "
                                  "interval INTEGER DEFAULT 0, "
                                  "CONSTRAINT MailPropertiesUnique UNIQUE (server, port, user, password, send_from, send_to) "
                                  //"CONSTRAINT MailPropertiesCheck CHECK ((SELECT COUNT(*) FROM MailProperties) = 0 ) "
                                  "); ")))
            //"user_id INTEGER REFERENCES Users(id) ON DELETE CASCADE, "
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }

        // creating picture table
        if (!q.exec(QLatin1String("CREATE TABLE IF NOT EXISTS PictureProperties("
                                  "id INTEGER PRIMARY KEY, "
                                  "interval INTEGER DEFAULT 0 "
                                  "); ")))
        {
            throw std::runtime_error(q.lastError().text().toStdString());
        }
    }

    void DatabaseManager::customEvent(QEvent *event)
    {
        if (event->type() == QEvent::Type(QEvent::User + 1))
        {
            databasetypes::DatabaseEvent *dEvent = static_cast<databasetypes::DatabaseEvent*>(event);
            databasetypes::IBaseDataPtr base = dEvent->data();

            switch (base->type())
            {
            case databasetypes::PictureEntity:
                storePicture(static_cast<databasetypes::PictureData&>(*base));
                break;

            case databasetypes::GetPicturesEntity:
                selectPictures(static_cast<databasetypes::GetPicturesData&>(*base), dEvent->sender());
                break;

            case databasetypes::AutorizationEntity:
                checkUser(static_cast<databasetypes::AutorizationData&>(*base), dEvent->sender());
                break;

            case databasetypes::MailPropertiesEntity:
                storeMailProperties(static_cast<databasetypes::MailPropertiesData&>(*base));
                break;

            case databasetypes::GetMailPropertiesEntity:
                selectMailProperties(dEvent->sender());
                break;

            case databasetypes::GetNewPictureEntity:
                selectNewPictures(dEvent->sender());
                break;

            case databasetypes::PicturePropertiesEntity:
                storePictureProperties(static_cast<databasetypes::PicturePropertiesData&>(*base));
                break;

            case databasetypes::GetPicturePropertiesEntity:
                selectPictureProperties(dEvent->sender());
                break;

            case databasetypes::UserEntity:
                storeUser(static_cast<databasetypes::UserData&>(*base), dEvent->sender());
                break;

            case databasetypes::GetUserEntity:
                selectUsers(dEvent->sender());
                break;

            case databasetypes::DeleteUserEntity:
                deleteUser(static_cast<databasetypes::DeleteUserData&>(*base), dEvent->sender());
                break;

            case databasetypes::SettingsEntity:
                storeSettings(static_cast<databasetypes::SettingsData&>(*base));
                break;

            case databasetypes::GetSettingsEntity:
                selectSettings(dEvent->sender());
                break;

            default:
                Q_ASSERT("You shouldn't be here" && false);
                break;
            }

        }
        else
        {
            QObject::customEvent(event);
        }
    }

    void DatabaseManager::storePicture(const databasetypes::PictureData &picture)
    {
        QSqlQuery q;

        // write pixmap in bytes
        QByteArray bytes;
        QBuffer buf(&bytes);
        buf.open(QIODevice::WriteOnly);
        picture.picture().save(&buf, "png");

        QDateTime date(picture.date());

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":picture"), bytes);
        placeholders.insert(QLatin1String(":date"), date);

        runQuerry(q, QLatin1String("INSERT INTO Pictures(picture, date)"
                                   "VALUES(:picture, strftime('%Y-%m-%d %H:%M:%f', :date)); "),
                  placeholders);
    }

    void DatabaseManager::selectPictures(const databasetypes::GetPicturesData& data, QObject *sender)
    {
        QSqlQuery q;

        QString querryString(QLatin1String("SELECT picture, date FROM Pictures "));

        QDateTime firstDate = data.firstDate();
        QDateTime secondDate = data.secondDate();

        if (secondDate.isValid())
        {
            querryString += QString(QLatin1String("WHERE datetime(date) BETWEEN datetime(\'%1\') AND datetime(\'%2\');"))
                    .arg(firstDate.toString(Qt::ISODate))
                    .arg(secondDate.toString(Qt::ISODate));
        }
        else if (firstDate.isValid())
        {
            querryString += QString(QLatin1String("WHERE date(date) = date(\'%1\');")).arg(firstDate.date().toString(Qt::ISODate));
        }
        else
        {
            querryString += QLatin1String(";");
        }

        runQuerry(q, querryString);

        while (q.next())
        {
            QByteArray bytes = q.value(0).toByteArray();
            QDateTime date = q.value(1).toDateTime();

            QImage img;
            bool check = img.loadFromData(bytes, "png");
            if (check)
            {
                databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::PictureData(img, date)), sender);
            }
            else
            {
                // ???
            }
        }

        databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::EndData()), sender);
    }

    void DatabaseManager::checkUser(const databasetypes::AutorizationData& data, QObject *receiver)
    {
        QSqlQuery q;
        QString user = data.user();
        QByteArray password = data.password();

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":user_name"), user);
        placeholders.insert(QLatin1String(":user_pwd"), password);

        runQuerry(q, QLatin1String("SELECT user_name, user_pwd "
                                   "FROM Users "
                                   "WHERE user_name = :user_name "
                                   "AND user_pwd = CAST(:user_pwd AS BLOB); "),
                             placeholders);

        databasetypes::IBaseDataPtr pData;
        if (q.first())
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData());
        }
        else
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData(databasetypes::ExecutionReportData::NOITEMS));
        }

        databasetypes::sendDatabaseEvent(pData, receiver);
    }

    void DatabaseManager::storeMailProperties(const databasetypes::MailPropertiesData &mp)
    {
        QSqlQuery q;
        databasetypes::MailParams params(mp.mailParams());

        // srart transaction
        runQuerry(q, QLatin1String("BEGIN TRANSACTION"));

        if (params.interval > 0)
        {
            // deletes and after insert new values
            runQuerry(q, QLatin1String("DELETE FROM MailProperties; "));

            QMap<QString, QVariant> placeholders;
            placeholders.insert(QLatin1String(":server"), params.server);
            placeholders.insert(QLatin1String(":port"), params.serverPort);
            placeholders.insert(QLatin1String(":user"), params.user);
            placeholders.insert(QLatin1String(":password"), params.password);
            placeholders.insert(QLatin1String(":send_from"), params.sendFrom);
            placeholders.insert(QLatin1String(":send_to"), params.sendTo.join(QLatin1String("\n")));
            placeholders.insert(QLatin1String(":interval"), params.interval);

            runQuerry(q, QLatin1String("INSERT INTO MailProperties(server, port, user, password, send_from, send_to, interval) "
                                       "VALUES(:server, :port, :user, :password, :send_from, :send_to, :interval); "),
                      placeholders);
        }
        else
        {       
            runQuerry(q, QLatin1String("UPDATE MailProperties "
                                       "SET interval = 0"));
        }

        // commit transaction
        runQuerry(q, QLatin1String("COMMIT TRANSACTION;"));
    }

    void DatabaseManager::selectMailProperties(QObject *receiver)
    {
        QSqlQuery q;

        runQuerry(q, QLatin1String("SELECT server, port, user, password, send_from, send_to, interval "
                                   "FROM MailProperties "));

        databasetypes::MailParams mp;
        if (q.first())
        {
            mp.server = q.value(0).toString();
            mp.serverPort = q.value(1).toInt();
            mp.user = q.value(2).toString();
            mp.password = q.value(3).toString();
            mp.sendFrom = q.value(4).toString();
            mp.sendTo = q.value(5).toString().split(QLatin1String("\n"), QString::SkipEmptyParts);
            mp.interval = q.value(6).toInt();
        }

        databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::MailPropertiesData(mp)), receiver);
    }

    void DatabaseManager::selectNewPictures(QObject *receiver)
    {
        QSqlQuery q;

        // select new pictures
        runQuerry(q, QLatin1String("SELECT picture, date "
                                 "FROM Pictures "
                                 "WHERE state = 0; "));

        while (q.next())
        {
            QByteArray bytes = q.value(0).toByteArray();
            QDateTime date = q.value(1).toDateTime();

            QImage img;
            bool check = img.loadFromData(bytes, "png");
            if (check)
            {
                // send data
                databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::PictureData(img, date)), receiver);
            }
            else
            {
                // ???
            }
        }

        // make new pictures old
        runQuerry(q, QLatin1String("UPDATE Pictures "
                                 "SET state = 1 "
                                 "WHERE state = 0; "));

        // send end of transmittion data
        databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::EndData()), receiver);
    }

    void DatabaseManager::runQuerry(QSqlQuery& sqlQuerry, const QString &querry, const QMap<QString, QVariant> &placeholders)
    {
        if (placeholders.isEmpty())
        {
            // simple querry
            if (!sqlQuerry.exec(querry))
            {
                qDebug() << sqlQuerry.lastError().text();
            }
        }
        else
        {
            sqlQuerry.prepare(querry);

            for (QMap<QString, QVariant>::const_iterator it = placeholders.begin(); it != placeholders.end(); ++it)
            {
                sqlQuerry.bindValue(it.key(), it.value());
            }

            if (!sqlQuerry.exec())
            {
                qDebug() << sqlQuerry.lastError().text();
            }
        }
    }

    void DatabaseManager::storePictureProperties(const databasetypes::PicturePropertiesData &pp)
    {
        QSqlQuery q;
        int interval = pp.interval();

        // start transaction
        runQuerry(q, QLatin1String("BEGIN TRANSACTION;"));

        // deletes and after insert new values
        runQuerry(q, QLatin1String("DELETE FROM PictureProperties; "));

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":interval"), interval);

        runQuerry(q, QLatin1String("INSERT INTO PictureProperties(interval) "
                                   "VALUES(:interval); "),
                  placeholders);

        // commit transaction
        runQuerry(q, QLatin1String("COMMIT TRANSACTION;"));
    }

    void DatabaseManager::selectPictureProperties(QObject *receiver)
    {
        QSqlQuery q;

        runQuerry(q, QLatin1String("SELECT interval "
                                   "FROM PictureProperties "));

        int interval = 0;

        if (q.first())
        {
            interval = q.value(0).toInt();
        }

        databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::PicturePropertiesData(interval)), receiver);
    }

    void DatabaseManager::storeUser(databasetypes::UserData &userData, QObject *receiver)
    {
        // simple check
        if (userData.user().isEmpty() || userData.password().isEmpty())
        {
            databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData(databasetypes::ExecutionReportData::BAD)), receiver);
            return;
        }

        QSqlQuery q;

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":user_name"), userData.user());
        placeholders.insert(QLatin1String(":user_pwd"), userData.password());

        runQuerry(q, QLatin1String("INSERT INTO Users(user_name, user_pwd) "
                                   "VALUES(:user_name, :user_pwd); "),
                placeholders);

        databasetypes::IBaseDataPtr pData;
        if (q.isActive())
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData());
        }
        else
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData(databasetypes::ExecutionReportData::DUPLICATEITEMS));
        }

        databasetypes::sendDatabaseEvent(pData, receiver);
    }

    void DatabaseManager::selectUsers(QObject *receiver)
    {
        QSqlQuery q;
        q.setForwardOnly(true);

        runQuerry(q, QString(QLatin1String("SELECT user_name, user_pwd "
                                           "FROM Users ")));

        databasetypes::IBaseDataPtr pData;
        while (q.next())
        {
            QString user = q.value(0).toString();
            QByteArray password = q.value(1).toByteArray();

            databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::UserData(user, password)), receiver);
        }

        databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::EndData()), receiver);
    }

    void DatabaseManager::deleteUser(databasetypes::DeleteUserData &userData, QObject *receiver)
    {
        // simple check
        if (userData.userName().isEmpty())
        {
            databasetypes::sendDatabaseEvent(databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData(databasetypes::ExecutionReportData::BAD)), receiver);
            return;
        }

        QSqlQuery q;

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":user_name"), userData.userName());

        runQuerry(q, QLatin1String("DELETE FROM Users "
                                   "WHERE user_name = :user_name; "),
                  placeholders);

        databasetypes::IBaseDataPtr pData;
        if (q.isActive())
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData());
        }
        else
        {
            pData = databasetypes::IBaseDataPtr(new databasetypes::ExecutionReportData(databasetypes::ExecutionReportData::BAD));
        }

        databasetypes::sendDatabaseEvent(pData, receiver);
    }

    void DatabaseManager::storeSettings(databasetypes::SettingsData &settingsData)
    {
        QSqlQuery q;
        int port = settingsData.port();
        bool trayIcon = settingsData.trayIcon();
        bool startupMode = settingsData.startupMode();

        // start transaction
        runQuerry(q, QLatin1String("BEGIN TRANSACTION;"));

        // deletes and after insert new values
        runQuerry(q, QLatin1String("DELETE FROM Settings; "));

        QMap<QString, QVariant> placeholders;
        placeholders.insert(QLatin1String(":port"), port);
        placeholders.insert(QLatin1String(":tray_icon"), trayIcon);
        placeholders.insert(QLatin1String(":startup_mode"), startupMode);

        runQuerry(q, QLatin1String("INSERT INTO Settings(port, tray_icon, startup_mode) "
                                   "VALUES(:port, :tray_icon, :startup_mode); "),
                  placeholders);

        // commit transaction
        runQuerry(q, QLatin1String("COMMIT TRANSACTION;"));
    }

    void DatabaseManager::selectSettings(QObject *receiver)
    {
        QSqlQuery q;

        runQuerry(q, QLatin1String("SELECT port, tray_icon, startup_mode "
                                   "FROM Settings; "));

        databasetypes::IBaseDataPtr pData;
        if (q.first())
        {
            int dbPort = q.value(0).toInt();
            bool dbTrayIcon = q.value(1).toBool();
            bool dbStartupMode = q.value(2).toBool();
            pData = databasetypes::IBaseDataPtr(new databasetypes::SettingsData(dbPort, dbTrayIcon, dbStartupMode));
        }
        else
        {
            // default parameters
            pData = databasetypes::IBaseDataPtr(new databasetypes::SettingsData(7777, true, true));
        }

        databasetypes::sendDatabaseEvent(pData, receiver);
    }
} // database
