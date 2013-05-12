
#include "clipboardquery.h"

namespace database
{
    namespace databasequery
    {
        StoreClipboardQuery::StoreClipboardQuery(const QVariant& clipboardData, ClipboardSingleton::ClipboardType clipboardType)
            : clipboardData_(clipboardData)
            , clipboardType_(clipboardType)
        {
        }

        databasetypes::IBaseDataPtr StoreClipboardQuery::data()
        {
            return databasetypes::IBaseDataPtr(new databasetypes::ClipboardData(clipboardData_, clipboardType_));
        }
    } // databasequery
} // database
