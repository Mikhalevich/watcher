#ifndef CLIPBOARD_DATABASE_QUERY_H
#define CLIPBOARD_DATABASE_QUERY_H

#include "abstractdatabasequery.h"

namespace database
{
    namespace databasequery
    {
        
        class StoreClipboardQuery : public IDatabaseWriter
        {
        public:
            StoreClipboardQuery(const QVariant& clipboardData, ClipboardSingleton::ClipboardType clipboardType);

        protected:
            virtual databasetypes::IBaseDataPtr data();

        private:
            QVariant clipboardData_;
            ClipboardSingleton::ClipboardType clipboardType_;
        };
    } // database
} // database

#endif // CLIPBOARD_DATABASE_QUERY_H
