
#include <QSharedPointer>
#include <QPixmap>

#include "tcpsocket.h"
#include "tcpserver.h"
#include "query/networkquery.h"
#include "query/picturequery.h"
#include "query/allpicturesquery.h"
#include "query/currentdatepicturesquery.h"
#include "query/periodpicturesquery.h"
#include "query/autorizationquery.h"
#include "query/timedpicturesquery.h"
#include "query/mailpropertiesquery.h"
#include "query/getmailpropertiesquery.h"
#include "query/getpicturepropertiesquery.h"
#include "query/settingsquery.h"

namespace network
{
    TcpSocket::TcpSocket(QObject *parent)
        : QTcpSocket(parent)
        , operation_(NOOPERATION)
        , size_(0)
        , autorization_(false)

    {
        connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
        //connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));

        fillFactory();
    }

    void TcpSocket::fillFactory()
    {
        queryFactory_.add<networkquery::PictureQuery>(GETPICTURE);
        queryFactory_.add<networkquery::AllPicturesQuery>(GETALLPICTURES);
        queryFactory_.add<networkquery::CurrentDatePicturesQuery>(GETCURRENTDATEPICTURES);
        queryFactory_.add<networkquery::PeriodPicturesQuery>(GETPERIODPICTURES);
        queryFactory_.add<networkquery::TimedPicturesQuery>(PICTURETIMER);
        queryFactory_.add<networkquery::GetPicturePropertiesQuery>(GETPICTURETIMER);
        queryFactory_.add<networkquery::MailPropertiesQuery>(MAILPROPERTIES);
        queryFactory_.add<networkquery::GetMailPropertiesQuery>(GETMAILPROPERTIES);
        queryFactory_.add<networkquery::GetSettingsQuery>(GETSETTINGS);
        queryFactory_.add<networkquery::SetSettingsQuery>(SETSETTINGS);
    }

    void TcpSocket::readData()
    {
        // endless cycle
        while (true)
        {
            if (operation_ == NOOPERATION)
            {
                if (bytesAvailable() < (sizeof(operation_) + sizeof(size_)))
                {
                    // wait when all bytes come
                    return;
                }

                QDataStream in(this);
                in >> operation_ >> size_;
            }

            if (size_ > 0)
            {
                if (bytesAvailable() < size_)
                {
                    // wait when all bytes come
                    return;
                }
            }

    #ifdef DEBUG
        qDebug() << operation_ << size_;
    #endif

            QSharedPointer<networkquery::INetworkQuery> query;

            if (!autorization_)
            {
                if (operation_ != AUTORIZATION)
                {
                    if (size_ > 0)
                    {
                        // remove bytes
                        read(size_);
                    }

                    // wait a new command (should be a AUTORIZATION)
                    operation_ = NOOPERATION;
                    size_ = 0;

                    return;
                }

                query = QSharedPointer<networkquery::INetworkQuery>(new networkquery::AutorizationQuery());
                connect(query.data(), SIGNAL(notifyAutorization(bool)), this, SLOT(setAutorization(bool)), Qt::QueuedConnection);
            }
            else
            {
                // create concreate querry
                query = queryFactory_.create(static_cast<Operation>(operation_));
            }

            //Q_ASSERT(querry);

            if (query)
            {
                try
                {
                    query->readData(this);
                }
                catch(const std::exception& /*e*/)
                {
                    // wait when all data come
                    return;
                }

                query->moveToThread(&queryThread_);
                connect(query.data(), SIGNAL(writeData(const QByteArray&)), this, SLOT(writeData(const QByteArray&)), Qt::QueuedConnection);
                queryThread_.addQuery(query);
            }

            // wait a new command
            operation_ = NOOPERATION;
            size_ = 0;
        }
    }

    void TcpSocket::writeData(const QByteArray &data)
    {
        write(data.data(), data.size());
    }

    void TcpSocket::setAutorization(bool state)
    {
        autorization_ = state;
    }
} // network
