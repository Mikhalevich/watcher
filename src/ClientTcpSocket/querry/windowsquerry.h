#ifndef WINDOWSQUERRY_H
#define WINDOWSQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class WindowsQuerry : public AbstractQuerry
        {
            Q_OBJECT

        public:
            WindowsQuerry();

            virtual void execute(QTcpSocket* socket);
        };
    } // networkquery
} // clientsocket

#endif // WINDOWSQUERRY_H
