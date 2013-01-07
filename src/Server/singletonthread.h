#ifndef DATABASETHREAD_H
#define DATABASETHREAD_H

#include <QThread>
#include <QAtomicInt>

class BaseSingletonThread : public QThread
{
    //Q_OBJECT

public:
    BaseSingletonThread(QObject *parent = 0)
        : QThread(parent)
        , initialize_(0)
    {
    }

    void waitWhileInitialize()
    {
        while (true)
        {
            if (initialize_ == 1)
            {
                return;
            }
            else
            {
                QThread::msleep(1);
            }
        }
    }

protected:
    void initialized()
    {
        bool status = initialize_.testAndSetAcquire(0, 1);
        Q_ASSERT(status);
    }

private:
    QAtomicInt initialize_;
};

template <typename T>
class SingletonThread : public BaseSingletonThread
{
    // no need signals and slots
    //Q_OBJECT

public:
    SingletonThread(QObject *parent = 0)
        : BaseSingletonThread(parent)
    {
    }

protected:
    virtual void run()
    {
        // create singleton in this thread
        // it must be a first time call
        T::instance();

        //
        initialized();

        // run event loop
        exec();
    }
};

#endif // DATABASETHREAD_H
