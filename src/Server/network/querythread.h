#ifndef QUERRYTHREAD_H
#define QUERRYTHREAD_H

#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

namespace network
{
    namespace networkquery
    {
        class INetworkQuery;
    }
}

namespace network
{
    class QueryThread : public QThread
    {
        Q_OBJECT

    private:
        typedef QSharedPointer<networkquery::INetworkQuery> INetworkQueryPtr;

    public:
        QueryThread();
        ~QueryThread();

        void addQuery(INetworkQueryPtr query);

    protected:
        virtual void run();

    private:
        QQueue<INetworkQueryPtr > querys_;
        QMutex queryMutex_;
        QWaitCondition emptyQuerys_;
    };
} // network

#endif // QUERRYTHREAD_H
