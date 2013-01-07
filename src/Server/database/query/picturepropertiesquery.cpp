
#include "picturepropertiesquery.h"

namespace database
{
    namespace databasequery
    {
        databasetypes::IBaseDataPtr GetPicturePropertiesQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::RequestData(databasetypes::GetPicturePropertiesEntity));
        }

        IDatabaseReaderImplPtr GetPicturePropertiesQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new SyncReader(new databasereaderimpl::SyncReportReaderImpl()));
        }

        const qint16 GetPicturePropertiesQuery::interval() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.interval();
        }

        GetPicturePropertiesQuery::SyncReader::SyncReader(IDatabaseReaderImpl *impl)
            : DatabaseReaderImplDecorator(impl)
        {
        }

        void GetPicturePropertiesQuery::SyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            if (base->type() == databasetypes::PicturePropertiesEntity)
            {
                transmitPictureProperties(static_cast<databasetypes::PicturePropertiesData&>(*base));
            }
            else
            {
                DatabaseReaderImplDecorator::execute(base);
            }
        }

        void GetPicturePropertiesQuery::SyncReader::transmitPictureProperties(const databasetypes::PicturePropertiesData &pp)
        {
            interval_ = (qint16)pp.interval();

            // stop event loop
            emit quit();
        }

        const qint16 GetPicturePropertiesQuery::SyncReader::interval() const
        {
            return interval_;
        }
    } // databasequery
} // database
