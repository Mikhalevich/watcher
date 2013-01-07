#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include <QWidget>
#include <QEvent>
#include <QScopedPointer>

#include "clientsocket.h"
#include "responcedata.h"

namespace clientsocket
{
    class AbstractWidgetImpl;

    class AbstractWidget : public QWidget
    {
        Q_OBJECT

    public:
        AbstractWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

        //AbstractWidget(const AbstractWidget& widget);

        virtual ~AbstractWidget();

        void setClientSocket(IClientSocket* socket);

        IClientSocket& getClientSocket() const;

    public slots:
        virtual void readData(const responcedata::AbstractData&) = 0;
        virtual void retranslateUi() = 0;

    protected:
        // change language event
        virtual void changeEvent(QEvent *event);

    private:
        QScopedPointer<AbstractWidgetImpl> impl_;
    };
} // clientsocket

#endif // ABSTRACTWIDGET_H
