#ifndef GETCONTROLINFOQUERRY_H
#define GETCONTROLINFOQUERRY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class GetControlInfoQuery : public INetworkQuery
        {
        public:
            GetControlInfoQuery();

            virtual void execute();

        private:
            void sendControlInfo(const int controlIndex, const int controlType, QTcpSocket *socket);
        };
    } // networkquery
} // netwok

#endif // GETCONTROLINFOQUERRY_H
