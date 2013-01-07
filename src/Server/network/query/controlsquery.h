#ifndef CONTROLSQUERRY_H
#define CONTROLSQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class ControlsQuery : public INetworkQuery
        {
        public:
            ControlsQuery();

            virtual void execute();
        };
    } // networkquery
} // network

#endif // CONTROLSQUERRY_H
