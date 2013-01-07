#ifndef GETMAILPROPERTIESQUERRY_H
#define GETMAILPROPERTIESQUERRY_H

#include "networkquery.h"
#include "databasetypes.h"

namespace network
{
    namespace networkquery
    {
        class GetMailPropertiesQuery : public INetworkQuery
        {
        public:
            virtual void execute();

        private:
            void transmitMailProperties(const database::databasetypes::MailParams& mp);
            void transmitError(const QString& error);
        };
    } // networkquery
} // network

#endif // GETMAILPROPERTIESQUERRY_H
