#ifndef GETCONTROLINFOQUERRY_H
#define GETCONTROLINFOQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class GetControlInfoQuerry : public AbstractQuerry
        {
            Q_OBJECT

        public:
            GetControlInfoQuerry();

            virtual void execute(QTcpSocket* socket);
        };
    } // networkquery
} // clientsocket

#endif // GETCONTROLINFOQUERRY_H
