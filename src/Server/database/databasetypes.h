#ifndef DATABASETYPES_H
#define DATABASETYPES_H

#include <QPixmap>
#include <QEvent>
#include <QDateTime>
#include <QCoreApplication>

namespace database
{
    namespace databasetypes
    {
        enum EntityType {

            EndEntity,

            PictureEntity,
            GetPicturesEntity,

            GetNewPictureEntity,

            UserEntity,
            GetUserEntity,
            DeleteUserEntity,

            AutorizationEntity,

            ExecutionReportEntity,

            MailPropertiesEntity,
            GetMailPropertiesEntity,

            PicturePropertiesEntity,
            GetPicturePropertiesEntity,

            SettingsEntity,
            GetSettingsEntity

        };
        //////////////////////////////////////////////////////////////////////////////////////////////

        // base hierarchy class
        class IBaseData
        {
        public:
            // ALWAYS use class with virtual destructor BLEAT'
            virtual ~IBaseData()
            {
            }

            virtual const EntityType type() const = 0;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        /* determinates the end of transmitting series of data */
        class EndData : public IBaseData
        {
        public:
            virtual const EntityType type() const
            {
                return EndEntity;
            }
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        /* all simple request querrys */
        /* work with Get-prefix entity */
        class RequestData : public IBaseData
        {
        public:
            RequestData(const EntityType type)
                : type_(type)
            {
            }

            virtual const EntityType type() const
            {
                return type_;
            }

        private:
            const EntityType type_;
        };
        ///////////////////////////////////////////////////////////////////////////////////////////////////

        class ExecutionReportData : public IBaseData
        {
        public:

            enum ExecutionStatus
            {
                GOOD = 0,
                NOITEMS,
                DUPLICATEITEMS,
                BAD
            };

            ExecutionReportData(ExecutionStatus status = GOOD)
                : status_(status)
            {
            }

            virtual const EntityType type() const
            {
                return ExecutionReportEntity;
            }

            operator bool() const
            {
                return status_ == GOOD;
            }

            const ExecutionStatus status() const
            {
                return status_;
            }

        private:
            ExecutionStatus status_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class PictureData : public IBaseData
        {
        public:
            PictureData(const QImage& picture, const QDateTime& date)
                : picture_(picture)
                , date_(date)
            {
            }

            virtual const EntityType type() const
            {
                return PictureEntity;
            }

            const QImage& picture() const
            {
                Q_ASSERT(!picture_.isNull());
                return picture_;
            }

            const QDateTime& date() const
            {
                return date_;
            }

        private:
            const QImage picture_;
            const QDateTime date_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class GetPicturesData : public IBaseData
        {
        public:
            GetPicturesData(const QDateTime& first = QDateTime(), const QDateTime& second = QDateTime())
                : first_(first)
                , second_(second)
            {
            }

            virtual const EntityType type() const
            {
                return GetPicturesEntity;
            }

            const QDateTime& firstDate() const
            {
                return first_;
            }

            const QDateTime& secondDate() const
            {
                return second_;
            }

        private:
            QDateTime first_;
            QDateTime second_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class UserData : public IBaseData
        {
        public:
            UserData(const QString& user, const QByteArray& password)
                : user_(user)
                , password_(password)
            {
            }

            virtual const EntityType type() const
            {
                return UserEntity;
            }

            const QString& user() const
            {
                return user_;
            }

            const QByteArray& password() const
            {
                return password_;
            }

        private:
            QString user_;
            QByteArray password_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class DeleteUserData : public IBaseData
        {
        public:
            DeleteUserData(const QString& userName)
                : userName_(userName)
            {
            }

            virtual const EntityType type() const
            {
                return DeleteUserEntity;
            }

            const QString& userName() const
            {
                return userName_;
            }

        private:
            QString userName_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class AutorizationData : public IBaseData
        {
        public:
            AutorizationData(const QString& user, const QByteArray& password)
                : user_(user)
                , password_(password)
            {
            }

            virtual const EntityType type() const
            {
                return AutorizationEntity;
            }

            const QString& user() const
            {
                return user_;
            }

            const QByteArray& password() const
            {
                return password_;
            }

        private:
            QString user_;
            QByteArray password_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        // additional struct for MailProperties class
        struct MailParams
        {
            MailParams()
                : interval(0)
                , serverPort(0)
            {
            }

            friend bool operator==(const MailParams& lhs, const MailParams& rhs);
            friend bool operator!=(const MailParams& lhs, const MailParams& rhs);

            QString server;
            qint32 serverPort;
            QString user;
            QString password;
            QString sendFrom;
            QStringList sendTo;
            int interval;
        };

        inline bool operator==(const MailParams& lhs, const MailParams& rhs)
        {
            if ((lhs.server == rhs.server)
                    && (lhs.serverPort == rhs.serverPort)
                    && (lhs.user == rhs.user)
                    && (lhs.password == rhs.password)
                    && (lhs.sendFrom == rhs.sendFrom)
                    && (lhs.sendTo == rhs.sendTo))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        inline bool operator!=(const MailParams& lhs, const MailParams& rhs)
        {
            return (!(lhs == rhs));
        }

        // request and respond
        class MailPropertiesData : public IBaseData
        {
        public:
            MailPropertiesData(MailParams mp)
                : mp_(mp)
            {
            }

            virtual const EntityType type() const
            {
                return MailPropertiesEntity;
            }

            const MailParams& mailParams() const
            {
                return mp_;
            }

        private:
            MailParams mp_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class PicturePropertiesData : public IBaseData
        {
        public:
            PicturePropertiesData(const int interval)
                : interval_(interval)
            {
            }

            virtual const EntityType type() const
            {
                return PicturePropertiesEntity;
            }

            const int interval() const
            {
                return interval_;
            }

        private:
            int interval_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        class SettingsData : public IBaseData
        {
        public:
            SettingsData(int port, bool trayIcon, bool startupMode)
                : port_(port)
                , trayIcon_(trayIcon)
                , startupMode_(startupMode)
            {
            }

            virtual const EntityType type() const
            {
                return SettingsEntity;
            }

            const int port() const
            {
                return port_;
            }

            const bool trayIcon() const
            {
                return trayIcon_;
            }

            const bool startupMode() const
            {
                return startupMode_;
            }

        private:
            int port_;
            bool trayIcon_;
            bool startupMode_;
        };

        // events answer
        class DatabaseEvent : public QEvent
        {
        public:
            DatabaseEvent(QSharedPointer<IBaseData> data, QObject *sender = 0)
                : QEvent(QEvent::Type(QEvent::User + 1))
                , data_(data)
                , sender_(sender)
            {
            }

            /*virtual ~AnswerEvent()
            {
            }*/

            QSharedPointer<IBaseData> data() const
            {
                return data_;
            }

            QObject* sender() const
            {
                return sender_;
            }

        private:
            QSharedPointer<IBaseData> data_;
            QObject *sender_;
        };
        //////////////////////////////////////////////////////////////////////////////////////////////////

        typedef QSharedPointer<IBaseData> IBaseDataPtr;

        // functin that send DatabaseEvent
        inline void sendDatabaseEvent(IBaseDataPtr pData, QObject *receiver, QObject *sender = 0)
        {
            databasetypes::DatabaseEvent *event = new databasetypes::DatabaseEvent(pData, sender);
            QCoreApplication::postEvent(receiver, event);
        }
    }
} // database

#endif // DATABASETYPES_H
