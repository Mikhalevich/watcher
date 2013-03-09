#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVariant>

#include "databasetypes.h"

class QPixmap;
class QSqlQuery;

namespace database
{
    class DatabaseManager : public QObject
    {
        Q_OBJECT

    public:
        //DatabaseManager(QObject *parent = 0);
        static DatabaseManager* instance()
        {
            static DatabaseManager* dbm;
            if (dbm == 0)
            {
                dbm = new DatabaseManager();
            }

            return dbm;
        }

    protected:
        virtual void customEvent(QEvent *event);

    private:
        DatabaseManager(QObject *parent = 0);
        DatabaseManager(const DatabaseManager&);
        DatabaseManager& operator=(const DatabaseManager&);

        void createDatabase();

        /* functions to work with database */
        void runQuerry(QSqlQuery& sqlQuerry, const QString& querry, const QMap<QString, QVariant>& placeholders = QMap<QString, QVariant>());

        void storePicture(const databasetypes::PictureData& picture);
        void selectPictures(const databasetypes::GetPicturesData& data, QObject *receiver);
        void checkUser(const databasetypes::AutorizationData& data, QObject *receiver);
        void storeMailProperties(const databasetypes::MailPropertiesData& mp);
        void selectMailProperties(QObject *receiver);
        void selectNewPictures(QObject *receiver);
        void storePictureProperties(const databasetypes::PicturePropertiesData& pp);
        void selectPictureProperties(QObject *receiver);
        void storeUser(databasetypes::UserData& userData, QObject *receiver);
        void selectUsers(QObject *receiver);
        void deleteUser(databasetypes::DeleteUserData& userData, QObject *receiver);
        void storeSettings(databasetypes::SettingsData& settingsData);
        void selectSettings(QObject *receiver);

        void storeClipboard(const databasetypes::ClipboardData& clipboard);
        void selectClipboard();

        QSqlDatabase database_;
    };
} // database

#endif // DATABASEMANAGER_H
