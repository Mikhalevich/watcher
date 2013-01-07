#ifndef PERIODPICTURESQUERRY_H
#define PERIODPICTURESQUERRY_H

#include <QDateTime>

#include "allpicturesquery.h"

namespace network
{
    namespace networkquery
    {
        class PeriodPicturesQuery : public AllPicturesQuery
        {
        public:
            PeriodPicturesQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            QDateTime firstDate_;
            QDateTime secondDate_;
        };
    } // networkquery
} //network

#endif // PERIODPICTURESQUERRY_H
