#ifndef GETPICTUREPROPERTIESQUERRY_H
#define GETPICTUREPROPERTIESQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class GetPicturePropertiesQuerry : public AbstractQuerry
        {
        public:
            GetPicturePropertiesQuerry();

            virtual void execute(QTcpSocket *socket);
        };
    } // networkquery
} // clientsocket

#endif // GETPICTUREPROPERTIESQUERRY_H
