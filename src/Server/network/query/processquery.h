#ifndef PROCESSQUERRY_H
#define PROCESSQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class ProcessQuery : public INetworkQuery
        {
        public:
            ProcessQuery();

            virtual void execute();
        };
    } // networkquery
} // network

#endif // PROCESSQUERRY_H
