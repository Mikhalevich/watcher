
#include <QSharedPointer>
#include <QBuffer>

#include "clienttcpsocket.h"
#include "responcedata.h"
#include "querry/abstractquerry.h"
#include "querry/picturequerry.h"
#include "querry/getmailpropertiesquerry.h"
#include "querry/executionreportquerry.h"
#include "querry/getpicturepropertiesquerry.h"
#include "querry/settingsquery.h"
#include "querry/clipboardquery.h"

namespace clientsocket
{
    class ClientTcpSocketPrivate
    {
    public:
        ClientTcpSocketPrivate(const int number = 0);

        void init();
        void fillFactory();

        ObjectFactory<Operation, networkquery::AbstractQuerry> m_querryFactory;

        int m_socketNumber;

        // outgoing
        global::types::operation_t m_operation;
        global::types::operation_size_t m_size;

        // incoming
        global::types::operation_t m_serverOperation;
        global::types::operation_size_t m_serverSize;
    };

    ClientTcpSocketPrivate::ClientTcpSocketPrivate(const int number /* = 0 */)
        : m_socketNumber(0)
        , m_operation(NOOPERATION)
        , m_serverOperation(NOOPERATION)
        , m_serverSize(0)
    {
        fillFactory();
    }

    void ClientTcpSocketPrivate::fillFactory()
    {
        //m_querryFactory.add<>(AUTORIZATION);
        m_querryFactory.add<networkquery::PictureQuerry>(GETPICTURE);
        m_querryFactory.add<networkquery::GetMailPropertiesQuerry>(GETMAILPROPERTIES);
        m_querryFactory.add<networkquery::ExecutionReportQuerry>(EXECUTIONREPORT);
        m_querryFactory.add<networkquery::GetPicturePropertiesQuerry>(GETPICTURETIMER);
        m_querryFactory.add<networkquery::GetSettingsQuery>(GETSETTINGS);
        m_querryFactory.add<networkquery::ClipboardQuery>(GETCLIPBOARD);
    }

    ClientTcpSocket::ClientTcpSocket(QObject *parent)
        : QTcpSocket(parent)
        , d_ptr(new ClientTcpSocketPrivate())
    {
        //connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(sendMessages()));
        //connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT(disconnect()));
        //connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
        //      this, SLOT(error(QAbstractSocket::SocketError)));
        connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
        //connect(&tcpSocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        //      this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    }

    ClientTcpSocket::ClientTcpSocket(int number, QObject *parent)
        : QTcpSocket(parent)
        , d_ptr(new ClientTcpSocketPrivate(number))
    {
        //connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(sendMessages()));
        //connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT(disconnect()));
        //connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
        //      this, SLOT(error(QAbstractSocket::SocketError)));
        connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
        //connect(&tcpSocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
        //      this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    }

    ClientTcpSocket::~ClientTcpSocket()
    {
        // empty
    }

    void ClientTcpSocket::setNumber(int number)
    {
        d_ptr->m_socketNumber = number;
    }

    int ClientTcpSocket::number() const
    {
        return d_ptr->m_socketNumber;
    }

    bool ClientTcpSocket::isConnected()
    {
        return state() == QAbstractSocket::ConnectedState;
    }

    void ClientTcpSocket::getPicture()
    {
        d_ptr->m_operation = GETPICTURE;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::getPictures(const QDateTime begin, const QDateTime end)
    {
        QDataStream out(this);

        if (!end.isValid())
        {
            if (!begin.isValid())
            {
                d_ptr->m_operation = GETALLPICTURES;
                d_ptr->m_size = 0;
                out << d_ptr->m_operation << d_ptr->m_size;
            }
            else
            {
                d_ptr->m_operation = GETCURRENTDATEPICTURES;

                // calculating size of command data
                d_ptr->m_size = sizeof(begin);

                out << d_ptr->m_operation << d_ptr->m_size << begin;
            }
        }
        else
        {
            d_ptr->m_operation = GETPERIODPICTURES;

            // calculating size of command data
            d_ptr->m_size = sizeof(begin) + sizeof(end);

            out << d_ptr->m_operation << d_ptr->m_size << begin << end;
        }

    }

    void ClientTcpSocket::pictureTimer(int value)
    {
        d_ptr->m_operation = PICTURETIMER;
        qint16 timerInterval = (qint16)value;
        d_ptr->m_size = sizeof(timerInterval);

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size << timerInterval;
    }

    void ClientTcpSocket::getPictureTimer()
    {
        d_ptr->m_operation = GETPICTURETIMER;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::login(const QString& user, const QByteArray& password)
    {
        d_ptr->m_operation = AUTORIZATION;

        QByteArray bytes;
        QBuffer buf(&bytes);
        buf.open(QIODevice::WriteOnly);

        QDataStream outBuf(&buf);
        outBuf << user.toUtf8() << password;
        buf.close();

        d_ptr->m_size = bytes.size();

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size << user.toUtf8() << password;

    }

    void ClientTcpSocket::mailProperties(const QString &server, const int port,
                                         const QString &user, const QString &password,
                                         const QString &sendFrom, const QStringList &sendTo,
                                         int interval)
    {
        d_ptr->m_operation = MAILPROPERTIES;

        QByteArray recepients = sendTo.join(QLatin1String(",")).toUtf8();

        // determinate size
        QByteArray bytes;
        QDataStream sizeBuf(&bytes, QIODevice::WriteOnly);
        sizeBuf << server.toUtf8() << (qint32)port
                << user.toUtf8() << password.toUtf8()
                << sendFrom.toUtf8() << recepients
                << (qint16)interval;

        d_ptr->m_size = bytes.size();

        // send to server
        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size
            << server.toUtf8() << (qint32)port
            << user.toUtf8() << password.toUtf8()
            << sendFrom.toUtf8() << recepients
            << (qint16)interval;
    }

    void ClientTcpSocket::getMailProperties()
    {
        d_ptr->m_operation = GETMAILPROPERTIES;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::getSettings()
    {
        d_ptr->m_operation = GETSETTINGS;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::setSettings(qint32 port, qint8 startupMode, qint8 trayIcon)
    {
        d_ptr->m_operation = SETSETTINGS;

        // determinate size
        QByteArray bytes;
        QDataStream sizeBuf(&bytes, QIODevice::WriteOnly);
        sizeBuf << (qint32)port << qint8(startupMode) << qint8(trayIcon);

        d_ptr->m_size = bytes.size();

        // send to server
        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size
            << (qint32)port << qint8(startupMode) << qint8(trayIcon);
    }

    void ClientTcpSocket::getClipboard()
    {
        d_ptr->m_operation = GETCLIPBOARD;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::setClipboard(const QVariant& clipboardData)
    {
        d_ptr->m_operation = SETCLIPBOARD;

        // determinate size
        QByteArray bytes;
        QDataStream sizeBuf(&bytes, QIODevice::WriteOnly);
        sizeBuf << clipboardData;

        d_ptr->m_size = bytes.size();

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size << clipboardData;
    }

    void ClientTcpSocket::getLastClipboard()
    {
        d_ptr->m_operation = GETLASTCLIPBOARD;
        d_ptr->m_size = 0;

        QDataStream out(this);
        out << d_ptr->m_operation << d_ptr->m_size;
    }

    void ClientTcpSocket::readData()
    {
        // endless cycle
        while (true)
        {
            QDataStream in(this);

            if (d_ptr->m_serverOperation == NOOPERATION)
            {
                if (bytesAvailable() < (sizeof(d_ptr->m_serverOperation) + sizeof(d_ptr->m_serverSize)))
                {
                    // wait all data
                    return;
                }

                // read operation and size
                in >> d_ptr->m_serverOperation >> d_ptr->m_serverSize;

                // end of transmittion
                if (d_ptr->m_serverSize == 0)
                {
                    //operation = NOOPERATION;
                    //size = 0;
                    // todo
                }
            }

            if (bytesAvailable() < d_ptr->m_serverSize)
            {
                // wait while all data come
                return;
            }

            QSharedPointer<networkquery::AbstractQuerry> querry;

            if (d_ptr->m_serverOperation == AUTORIZATION)
            {
                checkAutorization(this);
            }
            else
            {
                querry = d_ptr->m_querryFactory.create(static_cast<Operation>(d_ptr->m_serverOperation));
            }

            //Q_ASSERT(querry);

            if (querry)
            {
                connect(querry.data(), SIGNAL(sendData(const responcedata::AbstractData&)), this, SIGNAL(sendData(const responcedata::AbstractData&)));
                querry->execute(this);
            }

            d_ptr->m_serverOperation = NOOPERATION;
            d_ptr->m_serverSize = 0;
        }
    }

    void ClientTcpSocket::checkAutorization(QTcpSocket *socket)
    {
        QByteArray bytes;
        qint8 autorization;

        QDataStream in(socket);
        in >> autorization >> bytes;

        if (autorization == 0)
        {
            emit sendAutorization(true);
        }
        else
        {
            QString error = QString::fromUtf8(bytes.data());

            emit sendAutorization(false, error);

            // disconnect socket
            disconnectFromHost();
        }
    }
} // clientsocket
