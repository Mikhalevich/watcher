#ifndef TIMEDPICTURES_H
#define TIMEDPICTURES_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class TimedPicturesQuery : public INetworkQuery
        {   
        public:
            TimedPicturesQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            qint16 interval_;
        };
    } // networkquery
} // network

#endif // TIMEDPICTURES_H
