#ifndef AUTORIZATIONQUERY_H
#define AUTORIZATIONQUERY_H

#include "abstractdatabasequery.h"
#include "databasequerycommon.h"

namespace database
{
    namespace databasequery
    {
        class AutorizationQuery : public databasereader::DatabaseReaderReport
        {
        public:
            AutorizationQuery(const QString& user, const QByteArray& password);

        protected:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

        private:
            QString user_;
            QByteArray password_;
        };
    } // namespace databasequery
} // database

#endif // AUTORIZATIONQUERY_H