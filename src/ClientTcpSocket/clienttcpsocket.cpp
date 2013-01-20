
#include <QSharedPointer>
#include <QBuffer>

#include "clienttcpsocket.h"
#include "responcedata.h"
#include "querry/abstractquerry.h"
#include "querry/picturequerry.h"
#include "querry/windowsquerry.h"
#include "querry/controlsquerry.h"
#include "querry/getcontrolinfoquerry.h"
#include "querry/getmailpropertiesquerry.h"
#include "querry/executionreportquerry.h"
#include "querry/getpicturepropertiesquerry.h"

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
        querryFactory_.add<networkquery::WindowsQuerry>(GETALLWINDOWS);
        querryFactory_.add<networkquery::PictureQuerry>(GETPICTURE);
        querryFactory_.add<networkquery::ControlsQuerry>(GETALLCONTROLS);
        querryFactory_.add<networkquery::GetControlInfoQuerry>(GETCONTROLINFO);
        querryFactory_.add<networkquery::GetMailPropertiesQuerry>(GETMAILPROPERTIES);
        querryFactory_.add<networkquery::ExecutionReportQuerry>(EXECUTIONREPORT);
        querryFactory_.add<networkquery::GetPicturePropertiesQuerry>(GETPICTURETIMER);
    }

    bool ClientTcpSocket::isConnected()
    {
        return state() == QAbstractSocket::ConnectedState;
    }

    void ClientTcpSocket::getAllWindows()
    {
        operation_ = GETALLWINDOWS;
        size_ = 0;

        // send only one code
        QDataStream out(this);
        out << operation_ << size_;
    }

    void ClientTcpSocket::getAllControls(const QString& windowName)
    {
        operation_ = GETALLCONTROLS;

        QString window = windowName + QLatin1Char('\n');
        QByteArray bytes = window.toUtf8();

        // calculating size of command data
        size_ = bytes.size();

        // send code with window name
        QDataStream out(this);
        out << operation_ << size_;

        write(bytes);
    }

    void ClientTcpSocket::setControlText(qint16 controlIndex, qint8 controlType, const QString& newControlText)
    {
        operation_ = SETCONTROLTEXT;

        QString newText(newControlText);
        // replace all new line to number 2 character
        newText.replace(QLatin1Char('\n'), QLatin1Char(0x02));
        // add new line
        newText.append(QLatin1String("\n"));
        QByteArray bytes = newText.toUtf8();

        // calculating size of command data
        size_ = sizeof(controlIndex) + sizeof(controlType) + bytes.size();

        // send code with number of control, control type and new control text
        QDataStream out(this);
        out << operation_ << size_;
        out << controlIndex;
        out << controlType;

        write(bytes);
    }

    void ClientTcpSocket::getControlInfo(qint16 controlIndex, qint8 controlType)
    {
        operation_ = GETCONTROLINFO;

        // calculating size of command data
        size_ = sizeof(controlIndex) + sizeof(controlType);

        // send code with number of control, control type and new control text
        QDataStream out(this);
        out << operation_ << size_;
        out << controlIndex;
        out << controlType;
    }

    void ClientTcpSocket::runProcess(const QString& processLine)
    {
        operation_ = RUNPROCESS;

        QString line(processLine);
        line += QLatin1Char('\n');
        QByteArray bytes = line.toUtf8();

        // calculating size of command data
        size_ = bytes.size();

        QDataStream out(this);
        out << operation_ << size_;
        write(bytes);
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
