#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <QEvent>

#include "clienttcpsocket.h"

class AbstractWidget : public QWidget
{
    Q_OBJECT

public:
    AbstractWidget(QWidget *parent = 0, Qt::WindowFlags f = 0)
        : QWidget(parent, f)
        , socket_(0)
    {
    }

    AbstractWidget(const AbstractWidget& widget)
        : QWidget((QWidget*)this->parent(), this->windowFlags())
    {
    }

    virtual ~AbstractWidget()
    {
    }

    void setClientSocket(ClientTcpSocket* socket)
    {
        if (!socket_)
        {
            socket_ = socket;
            connect(socket_, SIGNAL(sendData(const AbstractData&)), this, SLOT(readData(const AbstractData&)));
        }
        else
        {
            Q_ASSERT("calling only once" && false);
        }
    }

    ClientTcpSocket& getClientSocket() const
    {
        Q_ASSERT(socket_);
        return *socket_;
    }

public slots:
    virtual void readData(const AbstractData&) = 0;
    virtual void retranslateUi() = 0;

protected:
    // change language event
    virtual void changeEvent(QEvent *event)
    {
            if (event->type() == QEvent::LanguageChange)
            {
                    retranslateUi();
            }
            QWidget::changeEvent(event);
    }

private:
    ClientTcpSocket *socket_;
};

#endif // ABSTRACTWIDGET_H
