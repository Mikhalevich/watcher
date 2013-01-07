#ifndef PICTUREPROPERTIESQUERY_H
#define PICTUREPROPERTIESQUERY_H

#include "abstractdatabasequery.h"
#include "databasequerycommon.h"

namespace database
{
    namespace databasequery
    {
        class GetPicturePropertiesQuery : public databasereader::DatabaseReaderReport
        {
            Q_OBJECT

        public:
            virtual databasetypes::IBaseDataPtr data();
            virtual IDatabaseReaderImplPtr readerImpl(IAsyncDataReceiver *receiver);

            const qint16 interval() const;

        private:

            class SyncReader : public databasereaderimpl::DatabaseReaderImplDecorator
            {
            public:
                SyncReader(IDatabaseReaderImpl *impl);

                virtual void execute(databasetypes::IBaseDataPtr base);

                const qint16 interval() const;

            private:
                void transmitPictureProperties(const databasetypes::PicturePropertiesData &pp);

                qint16 interval_;
            };
        };
    } // databasequery
} // database

#endif // PICTUREPROPERTIESQUERY_H