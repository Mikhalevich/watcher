#ifndef MAILQUERY_H
#define MAILQUERY_H

#include "abstractdatabasequery.h"
#include "databasequerycommon.h"

namespace database
{
    namespace databasequery
    {
        class StoreMailPropertiesQuery : public IDatabaseWriter
        {
        public:
            StoreMailPropertiesQuery(const databasetypes::MailParams& mp);

            virtual databasetypes::IBaseDataPtr data();

        private:
            databasetypes::MailParams mailParams_;
        };

        //////////////////////////////////////////////////////////////////////

        class GetMailPropertiesQuery : public databasereader::DatabaseReaderReport
        {
            Q_OBJECT

        public:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

            const databasetypes::MailParams& mailParams() const;

        private:

            class SyncReader : public databasereaderimpl::DatabaseReaderImplDecorator
            {
            public:
                SyncReader(IDatabaseReaderImpl *impl);

                virtual void execute(databasetypes::IBaseDataPtr base);

                const databasetypes::MailParams& mailParams() const;

            private:
                void transmitMailProperties(databasetypes::MailPropertiesData& mpd);

                databasetypes::MailParams mailParams_;
            };
        };
    } // databasequery
} // database

#endif // MAILQUERY_H