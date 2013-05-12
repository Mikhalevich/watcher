
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

namespace clipboardmodel
{
    ClipboardModel::ClipboardModel(QObject* parent /* = 0 */)
        : QAbstractListModel(parent)
    {
    }

    int ClipboardModel::rowCount(const QModelIndex& parentIndex /* = QModelIndex() */) const
    {
        return clipboardData_.count();
    }

    QVariant ClipboardModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        ClipboardElement element = clipboardData_.at(index.row());

        switch (role)
        {
        case Qt::DisplayRole:
            switch (element.type())
            {
            case QVariant::Image:
                element = QLatin1String("#Image#");
                break;

            default:
                // nothing
                break;
            }

            return element;

        default:
            // nothing
            break;
        }

        return QVariant();
    }

    void ClipboardModel::addClipboardData(const ClipboardElement& element)
    {
        beginInsertRows(QModelIndex(), clipboardData_.count(), clipboardData_.count());
        clipboardData_.push_back(element);
        endInsertRows();
    }

    void ClipboardModel::clearClipboardData()
    {
        clipboardData_.clear();
        reset();
    }

} // clipboardmodel

/////////////////////////////////////////////////////////////////////////

ClipboardWindow::ClipboardWindow(QWidget *parent) :
    BaseWidget(parent)
{   
    /* retranslte all visible strings */
    retranslateUi();

    QDeclarativeView *view = new QDeclarativeView(this);
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    // NEEDED FOR QUICK MODIFYING QML FILES!!!!!!!!!!!
    // REMOVE IT AFTER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //view->setSource(QUrl("qrc:/qml/clipboard.qml"));
    view->setSource(QUrl("clipboard.qml"));

    view->rootContext()->setContextProperty("clipboardWindow", this);
    view->rootContext()->setContextProperty("clipboardModel", &clipboardModel_);

    // connections
    QObject *root = view->rootObject();
    connect(root, SIGNAL(getClipboard()), this, SLOT(clipboard()));
    connect(root, SIGNAL(setClipboard(const QString&)), this, SLOT(setClipboard(const QString&)));
    connect(root, SIGNAL(getLastClipboard()), this, SLOT(lastClipboard()));

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

void ClipboardWindow::setClipboard(const QString& text)
{
    getClientSocket().setClipboard(text);
}

void ClipboardWindow::lastClipboard()
{
    clipboardModel_.clearClipboardData();
    getClientSocket().getLastClipboard();
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
            if (!varData.toString().isEmpty())
            {
                emit textData(varData.toString());
                clipboardModel_.addClipboardData(varData);
            }
            break;

        case QVariant::Image:
            emit imageData(varData);
            clipboardModel_.addClipboardData(varData);
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
