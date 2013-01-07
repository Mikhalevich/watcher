#ifndef SETCONTROLTEXTQUERRY_H
#define SETCONTROLTEXTQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class SetControlTextQuery : public INetworkQuery
        {
        public:
            SetControlTextQuery();

            virtual void execute();
        };
    } // networkquery
} // network

#endif // SETCONTROLTEXTQUERRY_H
