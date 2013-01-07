
#include "autorizationquery.h"

namespace database
{
    namespace databasequery
    {
        AutorizationQuery::AutorizationQuery(const QString& user, const QByteArray& password)
            : user_(user)
            , password_(password)
        {
        }

        databasetypes::IBaseDataPtr AutorizationQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::AutorizationData(user_, password_));
        }

        IDatabaseReaderImplPtr AutorizationQuery::readerImpl(IAsyncDataReceiver *receiver)
        {
            databasereaderimpl::SyncReportReaderImpl::Map map;
            map.insert(databasetypes::ExecutionReportData::NOITEMS, tr("Autorization failed"));

            return IDatabaseReaderImplPtr(new databasereaderimpl::SyncReportReaderImpl(map));
        }
    } // databasequery
} // database
