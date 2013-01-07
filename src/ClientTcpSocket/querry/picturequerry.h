#ifndef PICTUREQUERRY_H
#define PICTUREQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class PictureQuerry : public AbstractQuerry
        {
            Q_OBJECT

        public:
            PictureQuerry();

            virtual void execute(QTcpSocket* socket);
        };
    } // networkquery
} // clientsocket

#endif // PICTUREQUERRY_H
