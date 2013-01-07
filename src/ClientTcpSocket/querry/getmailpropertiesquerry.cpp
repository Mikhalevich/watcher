
#include "getmailpropertiesquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        GetMailPropertiesQuerry::GetMailPropertiesQuerry()
        {
        }

        void GetMailPropertiesQuerry::execute(QTcpSocket *socket)
        {
            QString server;
            qint32 serverPort;
            QString user;
            QString password;
            QString sendFrom;
            QStringList sendTo;
            qint16 interval;

            QByteArray bytes;
            QDataStream in(socket);

            // server
            in >> bytes;
            server = QString::fromUtf8(bytes.data());

            // server port
            in >> serverPort;

            // user
            in >> bytes;
            user = QString::fromUtf8(bytes.data());

            // password
            in >> bytes;
            password = QString::fromUtf8(bytes.data());

            // send from
            in >> bytes;
            sendFrom = QString::fromUtf8(bytes.data());

            // send to
            in >> bytes;
            sendTo = QString::fromUtf8(bytes.data()).split(QLatin1String(","), QString::SkipEmptyParts);

            // interval
            in >> interval;

            emit sendData(responcedata::MailPropertiesData(server, serverPort, user, password, sendFrom, sendTo, interval));
        }
    } // networkquery
} // clientsocket
