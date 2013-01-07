#ifndef DATABASEQUERY_H
#define DATABASEQUERY_H

#include <QSharedPointer>

#include "databasetypes.h"
#include "databasemanager.h"

namespace database
{
    namespace databasequery
    {
        // base class to access to database
        class IDatabaseQuery
        {
        public:
            virtual ~IDatabaseQuery()
            {
            }

            virtual void execute() = 0;
        };
        /////////////////////////////////////////////////////////////////////////

        // write only access like insert
        class IDatabaseWriter : public IDatabaseQuery
        {
        public:
            virtual void execute()
            {
                databasetypes::IBaseDataPtr pData = data();
                databasetypes::sendDatabaseEvent(pData, DatabaseManager::instance());
            }

        protected:
            // return actual data(request) uses in execute
            virtual databasetypes::IBaseDataPtr data() = 0;
        };
        /////////////////////////////////////////////////////////////////////////

        // interface for async receive data from database
        class IAsyncDataReceiver
        {
        public:
            virtual ~IAsyncDataReceiver()
            {
            }

            virtual void processData(databasetypes::IBaseDataPtr base) = 0;
        };

        class IDatabaseReaderImpl : public QObject
        {
            Q_OBJECT

        public:
            IDatabaseReaderImpl(IAsyncDataReceiver *receiver = 0)
                : receiver_(receiver)
            {
            }

            virtual ~IDatabaseReaderImpl()
            {
            }

            virtual void execute(databasetypes::IBaseDataPtr base) = 0;

        signals:
            void quit();

        protected:
            IAsyncDataReceiver *receiver_;
        };

        typedef QSharedPointer<IDatabaseReaderImpl> IDatabaseReaderImplPtr;

        // read/write access like insert/select
        class IDatabaseReader : public QObject, public IDatabaseWriter
        {
            Q_OBJECT

        public:
            IDatabaseReader()
            {
            }

            virtual void execute(IAsyncDataReceiver *receiver = 0)
            {
                // create implementation
                impl_ = readerImpl(receiver);
                // send request to database
                databasetypes::IBaseDataPtr pData = data();
                databasetypes::sendDatabaseEvent(pData, DatabaseManager::instance(), this);
                // and wait while all data come
                QEventLoop loop;
                connect(impl_.data(), SIGNAL(quit()), &loop, SLOT(quit()));
                loop.exec();
            }

        protected:
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver) = 0;

            // results from database comes here
            virtual void customEvent(QEvent* event)
            {
                if (event->type() == QEvent::Type(QEvent::User + 1))
                {
                    databasetypes::DatabaseEvent *dEvent = static_cast<databasetypes::DatabaseEvent*>(event);
                    databasetypes::IBaseDataPtr base = dEvent->data();

                    impl_->execute(base);
                }
                else
                {
                    QObject::customEvent(event);
                }
            }

        protected:
            IDatabaseReaderImplPtr impl_;
        };
        //////////////////////////////////////////////////////////////////////////////////

    } // databasequery
} // database

#endif // DATABASEQUERY_H