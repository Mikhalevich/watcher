#ifndef USERQUERY_H
#define USERQUERY_H

#include "abstractdatabasequery.h"
#include "databasequerycommon.h"

namespace database
{
    namespace databasequery
    {
        // class to store user to database
        class StoreUserQuery : public databasereader::DatabaseReaderReport
        {
            Q_OBJECT

        public:
            StoreUserQuery(const QString& userName, const QByteArray& password);

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:
            QString userName_;
            QByteArray password_;
        };
        /////////////////////////////////////////////////////////////////////////////////////////

        // class to get users from database
        class GetUserQuery : public IDatabaseReader
        {
            Q_OBJECT

        public:
            typedef QVector<QPair<QString, QByteArray> > Conteiner;

            const Conteiner& users() const;

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:

            class SyncReader : public IDatabaseReaderImpl
            {
            public:
                virtual void execute(databasetypes::IBaseDataPtr base);

                const Conteiner& users() const;

            private:
                void transmitUser(databasetypes::UserData& userData);
                void transmitEnd();

                Conteiner users_;
            };
        };
        /////////////////////////////////////////////////////////////////////////////////////////

        // class to delete user from database
        class DeleteUserQuery : public databasereader::DatabaseReaderReport
        {
            Q_OBJECT

        public:
            DeleteUserQuery(const QString& userName);

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:
            QString userName_;
        };
        /////////////////////////////////////////////////////////////////////////////////////////

    } // databasequery
} // database


#endif // USERQUERY_H