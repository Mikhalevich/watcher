
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QQuickView>
#include <QUrl>
#include <QQuickItem>
#include <QQmlContext>
#include <QWindow>

#include "clipboardwindow.h"
#include "clipboardmodel.h"

class ClipboardWindowPrivate
{
public:
    // variables
    ClipboardModel m_clipboardModel;
};

ClipboardWindow::ClipboardWindow(QWidget *parent)
    : BaseWidget(parent)
    , d_ptr(new ClipboardWindowPrivate())
{   
    /* retranslte all visible strings */
    retranslateUi();

    QQuickView *view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->rootContext()->setContextProperty("clipboardWindow", this);
    view->rootContext()->setContextProperty("clipboardModel", &d_ptr->m_clipboardModel);

    view->setSource(QUrl("qrc:/qml/clipboard.qml"));

    // connections
    QObject *root = view->rootObject();
    connect(root, SIGNAL(getClipboard()), this, SLOT(clipboard()));
    connect(root, SIGNAL(setClipboard(const QString&)), this, SLOT(setClipboard(const QString&)));
    connect(root, SIGNAL(getLastClipboard()), this, SLOT(lastClipboard()));
    connect(root, SIGNAL(clearModel()), this, SLOT(clearModel()));

#if QT_VERSION >= 0x050100
    // fix in 5.1
    QWidget *widget = QWidget::createWindowContainer(view);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(widget);
    
    setLayout(layout);
#else
    // before Qt5.1 use this spike
    view->show();
#endif // QT_VERSION >= 0x050100
}

ClipboardWindow::~ClipboardWindow()
{
    // empty
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
    d_ptr->m_clipboardModel.clearClipboardData();
    getClientSocket().getLastClipboard();
}

void ClipboardWindow::clearModel()
{
    d_ptr->m_clipboardModel.clearClipboardData();
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
                d_ptr->m_clipboardModel.addClipboardData(varData);
            }
            break;

        case QVariant::Image:
            emit imageData(varData);
            d_ptr->m_clipboardModel.addClipboardData(varData);
            break;

        default:
            QMessageBox::information(this, QApplication::applicationName(), tr("Can't define clipboard data"));
            break;
        }
    }
}
