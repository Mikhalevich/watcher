
#include "mailquery.h"

namespace database
{
    namespace databasequery
    {
        StoreMailPropertiesQuery::StoreMailPropertiesQuery(const databasetypes::MailParams& mp)
            : mailParams_(mp)
        {
        }

        databasetypes::IBaseDataPtr StoreMailPropertiesQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::MailPropertiesData(mailParams_));
        }

        /////////////////////////////////////////////////////////////////////////////////////

        databasetypes::IBaseDataPtr GetMailPropertiesQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::RequestData(databasetypes::GetMailPropertiesEntity));
        }

        IDatabaseReaderImplPtr GetMailPropertiesQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            return IDatabaseReaderImplPtr(new SyncReader(new databasereaderimpl::SyncReportReaderImpl()));
        }

        const databasetypes::MailParams& GetMailPropertiesQuery::mailParams() const
        {
            const SyncReader& impl = static_cast<const SyncReader&>(*impl_);
            return impl.mailParams();
        }

        GetMailPropertiesQuery::SyncReader::SyncReader(IDatabaseReaderImpl *impl)
            : DatabaseReaderImplDecorator(impl)
        {
        }

        void GetMailPropertiesQuery::SyncReader::execute(databasetypes::IBaseDataPtr base)
        {
            if (base->type() == databasetypes::MailPropertiesEntity)
            {
                transmitMailProperties(static_cast<databasetypes::MailPropertiesData&>(*base));
            }
            else
            {
                DatabaseReaderImplDecorator::execute(base);
            }
        }

        void GetMailPropertiesQuery::SyncReader::transmitMailProperties(databasetypes::MailPropertiesData& mpd)
        {
            mailParams_ = mpd.mailParams();

            // stop event loop
            emit quit();
        }

        const databasetypes::MailParams& GetMailPropertiesQuery::SyncReader::mailParams() const
        {
            return mailParams_;
        }
    } // databasequery
} // database
