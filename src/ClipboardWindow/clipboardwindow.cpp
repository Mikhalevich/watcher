
#include <QtPlugin>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>

#include "clipboardwindow.h"

ClipboardWindow::ClipboardWindow(QWidget *parent) :
    BaseWidget(parent)
{   
    /* retranslte all visible strings */
    retranslateUi();

    QPushButton *push = new QPushButton(tr("push"), this);
    connect(push, SIGNAL(clicked()), this, SLOT(push()));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(push);
    
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


void ClipboardWindow::readData(const AbstractData &data)
{
    if (data.type() == GETCLIPBOARD)
    {
        const clientsocket::responcedata::ClipboardData& clipboard = static_cast<const clientsocket::responcedata::ClipboardData&>(data);

        const QVariant& varData = clipboard.clipboardData();
        switch (varData.type())
        {
        case QVariant::String:
            QMessageBox::information(this, QString(), varData.toString());
            break;

        case QVariant::Image:
            {
                QLabel *label = new QLabel();
                label->setPixmap(QPixmap::fromImage(varData.value<QImage>()));
                label->show();
            }
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
