
#include <QtPlugin>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QDeclarativeView>
#include <QUrl>
#include <QGraphicsObject>
#include <QDeclarativeContext>

#include "clipboardwindow.h"

ClipboardWindow::ClipboardWindow(QWidget *parent) :
    BaseWidget(parent)
{   
    /* retranslte all visible strings */
    retranslateUi();

    QDeclarativeView *view = new QDeclarativeView(this);
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setSource(QUrl("qrc:/qml/clipboard.qml"));

    view->rootContext()->setContextProperty("clipboardWindow", this);

    // connections
    QObject *root = view->rootObject();
    connect(root, SIGNAL(getClipboard()), this, SLOT(clipboard()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(view);
    
    setLayout(layout);
}

QIcon ClipboardWindow::icon() const
{
    return QIcon(":/icons/resources/icons/scissors_32x32.png");
}

QString ClipboardWindow::text() const
{
    return tr("Clipboard Window");
}

QString ClipboardWindow::statusTip() const
{
    return tr("Clipboard Window");
}

void ClipboardWindow::retranslateUi()
{
}

void ClipboardWindow::clipboard()
{
    getClientSocket().getClipboard();
}

void ClipboardWindow::readData(const AbstractData &data)
{
    if (data.type() == GETCLIPBOARD)
    {
        const clientsocket::responcedata::ClipboardData& clipboard = static_cast<const clientsocket::responcedata::ClipboardData&>(data);

        const QVariant& varData = clipboard.clipboardData();
        switch (varData.type())
        {
        case QVariant::String:
            emit textData(varData.toString());
            break;

        case QVariant::Image:
            emit imageData(varData);
            break;

        default:
            QMessageBox::information(this, QApplication::applicationName(), tr("Can't define clipboard data"));
            break;
        }
    }
}

/* export plugin */
/* first parameter plugin name - see TARGET in .pro file */
/* second parameter plugin class - see class name in .h file */
Q_EXPORT_PLUGIN2(ClipboardWindow, ClipboardWindow)
