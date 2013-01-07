
#include "abstractwidget.h"
#include "clienttcpsocket.h"
#include "responcedata.h"

namespace clientsocket
{
    class AbstractWidgetImpl
    {
    public:
        AbstractWidgetImpl();

        void setSocket(IClientSocket *socket);
        IClientSocket& socket() const;

    private:
        IClientSocket *socket_;
    };

    AbstractWidgetImpl::AbstractWidgetImpl()
        : socket_(0)
    {
    }

    void AbstractWidgetImpl::setSocket(IClientSocket *socket)
    {
        if (!socket_)
        {
            socket_ = socket;
        }
        else
        {
            Q_ASSERT("calling only once" && false);
        }
    }

    IClientSocket& AbstractWidgetImpl::socket() const
    {
        Q_ASSERT(socket_);
        return *socket_;
    }

    AbstractWidget::AbstractWidget(QWidget *parent, Qt::WindowFlags f)
            : QWidget(parent, f)
            , impl_(new AbstractWidgetImpl())
    {
    }

    /*AbstractWidget::AbstractWidget(const AbstractWidget& widget)
            : QWidget((QWidget*)this->parent(), this->windowFlags())
    {
    }
    */

    AbstractWidget::~AbstractWidget()
    {
    }

    void AbstractWidget::setClientSocket(IClientSocket* socket)
    {
        impl_->setSocket(socket);
        ClientTcpSocket& socketImpl = static_cast<ClientTcpSocket&>(impl_->socket()); // bad!!!
        connect(&socketImpl, SIGNAL(sendData(const responcedata::AbstractData&)), this, SLOT(readData(const responcedata::AbstractData&)));
    }

    IClientSocket& AbstractWidget::getClientSocket() const
    {
        return impl_->socket();
    }

    void AbstractWidget::changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::LanguageChange)
        {
            retranslateUi();
        }
        QWidget::changeEvent(event);
    }
} // clientsocket
