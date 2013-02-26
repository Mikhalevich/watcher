#ifndef CLIPBOARD_QUERY_H
#define CLIPBOARD_QUERY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class ClipboardQuery : public INetworkQuery
        {
        public:
            ClipboardQuery();

            virtual void execute();
        };
    } // networkquery
} // network

#endif // CLIPBOARD_QUERY_H
