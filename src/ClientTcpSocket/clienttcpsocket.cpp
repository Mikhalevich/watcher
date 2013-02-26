
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
    ClientTcpSocket::ClientTcpSocket(QObject *parent)
        : QTcpSocket(parent)
        , socketNumber_(0)
        , operation_(NOOPERATION)
        , serverOperation_(NOOPERATION)
        , serverSize_(0)
    {
        init();
    }

    ClientTcpSocket::ClientTcpSocket(int number, QObject *parent)
        : QTcpSocket(parent)
        , socketNumber_(number)
        , operation_(NOOPERATION)
        , serverOperation_(NOOPERATION)
        , serverSize_(0)
    {
        init();
    }

    void ClientTcpSocket::init()
    {
        //connect(&tcpSocket_, SIGNAL(connected()), this, SLOT(sendMessages()));
        //connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT(disconnect()));
        //connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
          //      this, SLOT(error(QAbstractSocket::SocketError)));
        connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
        //connect(&tcpSocket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
          //      this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

        fillFactory();
    }

    void ClientTcpSocket::fillFactory()
    {
        //querryFactory_.add<>(AUTORIZATION);
        querryFactory_.add<networkquery::PictureQuerry>(GETPICTURE);
        querryFactory_.add<networkquery::GetMailPropertiesQuerry>(GETMAILPROPERTIES);
        querryFactory_.add<networkquery::ExecutionReportQuerry>(EXECUTIONREPORT);
        querryFactory_.add<networkquery::GetPicturePropertiesQuerry>(GETPICTURETIMER);
        querryFactory_.add<networkquery::GetSettingsQuery>(GETSETTINGS);
        querryFactory_.add<networkquery::ClipboardQuery>(GETCLIPBOARD);
    }

    bool ClientTcpSocket::isConnected()
    {
        return state() == QAbstractSocket::ConnectedState;
    }

    void ClientTcpSocket::getPicture()
    {
        operation_ = GETPICTURE;
        size_ = 0;

        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::getPictures(const QDateTime begin, const QDateTime end)
    {
        QDataStream out(this);

        if (!end.isValid())
        {
            if (!begin.isValid())
            {
                operation_ = GETALLPICTURES;
                size_ = 0;
                out << operation_ << size_;
            }
            else
            {
                operation_ = GETCURRENTDATEPICTURES;

                // calculating size of command data
                size_ = sizeof(begin);

                out << operation_ << size_ << begin;
            }
        }
        else
        {
            operation_ = GETPERIODPICTURES;

            // calculating size of command data
            size_ = sizeof(begin) + sizeof(end);

            out << operation_ << size_ << begin << end;
        }

    }

    void ClientTcpSocket::pictureTimer(int value)
    {
        operation_ = PICTURETIMER;
        qint16 timerInterval = (qint16)value;
        size_ = sizeof(timerInterval);

        QDataStream out(this);
        out << operation_ << size_ << timerInterval;
    }

    void ClientTcpSocket::getPictureTimer()
    {
        operation_ = GETPICTURETIMER;
        size_ = 0;

        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::login(const QString& user, const QByteArray& password)
    {
        operation_ = AUTORIZATION;

        QByteArray bytes;
        QBuffer buf(&bytes);
        buf.open(QIODevice::WriteOnly);

        QDataStream outBuf(&buf);
        outBuf << user.toUtf8() << password;
        buf.close();

        size_ = bytes.size();

        QDataStream out(this);
        out << operation_ << size_ << user.toUtf8() << password;

    }

    void ClientTcpSocket::mailProperties(const QString &server, const int port,
                                         const QString &user, const QString &password,
                                         const QString &sendFrom, const QStringList &sendTo,
                                         int interval)
    {
        operation_ = MAILPROPERTIES;

        QByteArray recepients = sendTo.join(QLatin1String(",")).toUtf8();

        // determinate size
        QByteArray bytes;
        QDataStream sizeBuf(&bytes, QIODevice::WriteOnly);
        sizeBuf << server.toUtf8() << (qint32)port
                << user.toUtf8() << password.toUtf8()
                << sendFrom.toUtf8() << recepients
                << (qint16)interval;

        size_ = bytes.size();

        // send to server
        QDataStream out(this);
        out << operation_ << size_
            << server.toUtf8() << (qint32)port
            << user.toUtf8() << password.toUtf8()
            << sendFrom.toUtf8() << recepients
            << (qint16)interval;
    }

    void ClientTcpSocket::getMailProperties()
    {
        operation_ = GETMAILPROPERTIES;
        size_ = 0;

        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::getSettings()
    {
        operation_ = GETSETTINGS;
        size_ = 0;

        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::setSettings(qint32 port, qint8 startupMode, qint8 trayIcon)
    {
        operation_ = SETSETTINGS;

        // determinate size
        QByteArray bytes;
        QDataStream sizeBuf(&bytes, QIODevice::WriteOnly);
        sizeBuf << (qint32)port << qint8(startupMode) << qint8(trayIcon);

        size_ = bytes.size();

        // send to server
        QDataStream out(this);
        out << operation_ << size_
            << (qint32)port << qint8(startupMode) << qint8(trayIcon);
    }

    void ClientTcpSocket::getClipboard()
    {
        operation_ = GETCLIPBOARD;
        size_ = 0;

        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::readData()
    {
        // endless cycle
        while (true)
        {
            QDataStream in(this);

            if (serverOperation_ == NOOPERATION)
            {
                if (bytesAvailable() < (sizeof(serverOperation_) + sizeof(serverSize_)))
                {
                    // wait all data
                    return;
                }

                // read operation and size
                in >> serverOperation_ >> serverSize_;

                // end of transmittion
                if (serverSize_ == 0)
                {
                    //operation = NOOPERATION;
                    //size = 0;
                    // todo
                }
            }

            if (bytesAvailable() < serverSize_)
            {
                // wait while all data come
                return;
            }

            QSharedPointer<networkquery::AbstractQuerry> querry;

            if (serverOperation_ == AUTORIZATION)
            {
                checkAutorization(this);
            }
            else
            {
                querry = querryFactory_.create(static_cast<Operation>(serverOperation_));
            }

            //Q_ASSERT(querry);

            if (querry)
            {
                connect(querry.data(), SIGNAL(sendData(const responcedata::AbstractData&)), this, SIGNAL(sendData(const responcedata::AbstractData&)));
                querry->execute(this);
            }

            serverOperation_ = NOOPERATION;
            serverSize_ = 0;
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
