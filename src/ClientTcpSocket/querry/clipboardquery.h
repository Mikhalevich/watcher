#ifndef CLIPBOARD_QUERY_H
#define CLIPBOARD_QUERY_H

#include "abstractquerry.h"

namespace clientsocket
{
    namespace networkquery
    {
        class ClipboardQuery : public AbstractQuerry
        {
            Q_OBJECT

        public:
            ClipboardQuery();

            virtual void execute(QTcpSocket* socket);

        private:
            QString text_; // temp
        };
    } // networkquery
} // clientsocket


#endif // CLIPBOARD_QUERY_H
