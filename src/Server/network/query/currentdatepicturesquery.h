#ifndef CURRENTPICTURESQUERRY_H
#define CURRENTPICTURESQUERRY_H

#include <QDateTime>

#include "allpicturesquery.h"

namespace network
{
    namespace networkquery
    {
        class CurrentDatePicturesQuery : public AllPicturesQuery
        {
        public:
            CurrentDatePicturesQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            QDateTime date_;
        };
    } // networkquery
} // network

#endif // CURRENTPICTURESQUERRY_H
