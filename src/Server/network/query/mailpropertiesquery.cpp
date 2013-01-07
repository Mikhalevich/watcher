
#include "mailpropertiesquery.h"
#include "picturetimer.h"

namespace network
{
    namespace networkquery
    {
        MailPropertiesQuery::MailPropertiesQuery()
        {
        }

        void MailPropertiesQuery::readData(TcpSocket *socket)
        {
            QByteArray server;
            QByteArray user;
            QByteArray password;
            QByteArray sendFrom;
            QByteArray recepients;

            QDataStream in(socket);
            in >> server >> serverPort_
                  >> user >> password
                  >> sendFrom >> recepients
                  >> mailInterval_;

            server_ = QString::fromUtf8(server.data());
            user_ = QString::fromUtf8(user.data());
            password_ = QString::fromUtf8(password.data());
            sendFrom_ = QString::fromUtf8(sendFrom.data());
            sendTo_ = QString::fromUtf8(recepients.data()).split(QLatin1String(","), QString::SkipEmptyParts);
        }

        void MailPropertiesQuery::execute()
        {
            QMetaObject::invokeMethod(PictureTimer::instance(), "mailProperties", Qt::QueuedConnection,
                                      Q_ARG(QString, server_),
                                      Q_ARG(int, (int)serverPort_),
                                      Q_ARG(QString, user_),
                                      Q_ARG(QString, password_),
                                      Q_ARG(QString, sendFrom_),
                                      Q_ARG(QStringList, sendTo_),
                                      Q_ARG(int, (int)mailInterval_));
        }
    } // networkquery
} // network
