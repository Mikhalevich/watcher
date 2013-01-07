#ifndef GETMAILPROPERTUESQUERRY_H
#define GETMAILPROPERTUESQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class GetMailPropertiesQuerry : public AbstractQuerry
        {
        public:
            GetMailPropertiesQuerry();

            virtual void execute(QTcpSocket *socket);
        };
    } // networkquery
} // clientsocket

#endif // GETMAILPROPERTUESQUERRY_H
