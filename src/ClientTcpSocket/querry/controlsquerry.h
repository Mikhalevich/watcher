#ifndef CONTROLSQUERRY_H
#define CONTROLSQUERRY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class ControlsQuerry : public AbstractQuerry
        {
            Q_OBJECT

        public:
            ControlsQuerry();

            virtual void execute(QTcpSocket* socket);

        private:
            QStringList allControls_;
        };
    } // networkquery
} // clientsocket

#endif // CONTROLSQUERRY_H
