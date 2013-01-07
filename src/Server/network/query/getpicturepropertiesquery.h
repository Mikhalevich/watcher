#ifndef GETPICTURESPROPERTIES_H
#define GETPICTURESPROPERTIES_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class GetPicturePropertiesQuery : public INetworkQuery
        {
        public:
            virtual void execute();

        private:
            void transmitPictureProperties(const qint16 interval);
            void transmitError(const QString& error);
        };
    } // networkquery
} // network

#endif // GETPICTURESPROPERTIES_H
