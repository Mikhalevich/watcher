
//#include <QMutexLocker>

#include "querythread.h"
#include "query/networkquery.h"

namespace network
{
    #define END INetworkQueryPtr()

    QueryThread::QueryThread()
    {
        start();
    }

    QueryThread::~QueryThread()
    {
        {
            QMutexLocker lock(&queryMutex_);

            while (querys_.size())
            {
                querys_.dequeue();
            }

            /* append the end of thread execution */
            querys_.enqueue(END);
            emptyQuerys_.wakeOne();
        }

        /* wait while thread die */
        wait();
    }

    void QueryThread::addQuery(INetworkQueryPtr query)
    {
        QMutexLocker lock(&queryMutex_);
        querys_.enqueue(query);
        emptyQuerys_.wakeOne();
    }

    void QueryThread::run()
    {
        while (true)
        {
            INetworkQueryPtr query;

            {
                QMutexLocker lock(&queryMutex_);

                if (querys_.isEmpty())
                {
                    emptyQuerys_.wait(&queryMutex_);
                }

                query = querys_.dequeue();
            }

            if (query == END)
            {
                break;
            }

            query->execute();
        }
    }
} // network
