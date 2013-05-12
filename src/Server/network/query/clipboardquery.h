#ifndef CLIPBOARD_QUERY_H
#define CLIPBOARD_QUERY_H

#include "networkquery.h"

namespace network
{
    namespace networkquery
    {
        class ClipboardQuery : public INetworkQuery
        {
        public:
            ClipboardQuery();

            virtual void execute();
        };

        ///////////////////////////////////////////////////////////////

        class SetClipdoardQuery : public INetworkQuery
        {
        public:
            SetClipdoardQuery();

            virtual void readData(TcpSocket *socket);
            virtual void execute();

        private:
            QVariant clipboardData_;
        };

        //////////////////////////////////////////////////////////////

        class GetLastClipboardQuery : public INetworkQuery
        {
        public:
            GetLastClipboardQuery();

            virtual void execute();
        };

    } // networkquery
} // network

#endif // CLIPBOARD_QUERY_H
