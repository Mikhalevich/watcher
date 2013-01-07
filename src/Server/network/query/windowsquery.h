#ifndef WINDOWSQUERRY_H
#define WINDOWSQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class WindowsQuery : public INetworkQuery
        {
        public:
            WindowsQuery();

            virtual void execute();
        };
    } // networkquery
} // network

#endif // WINDOWSQUERRY_H
