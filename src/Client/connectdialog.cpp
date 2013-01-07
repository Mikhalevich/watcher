
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

#include "connectdialog.h"

ConnectDialog::ConnectDialog(QString host, int port, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    //this->setAttribute(Qt::WA_DeleteOnClose);

    QLabel *hostLabel = new QLabel(tr("host"), this);
    QLabel *portLabel = new QLabel(tr("port"), this);

    host_ = new QLineEdit(host, this);
    port_ = new QLineEdit(QString::number(port), this);
    port_->setValidator(new QIntValidator());

    QDialogButtonBox *box = new QDialogButtonBox(Qt::Horizontal, this);
    QPushButton *okButton = box->addButton(QDialogButtonBox::Ok);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = box->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(hostLabel, 0, 0);
    layout->addWidget(host_, 0, 1);
    layout->addWidget(portLabel, 1, 0);
    layout->addWidget(port_, 1, 1);
    layout->addWidget(box, 2, 0, 1, 2);

    host_->setFocus();
}

void ConnectDialog::accept()
{
    if (host_->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input error"), tr("Enter host"));
        host_->setFocus();
        return;
    }

    if (port_->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input error"), tr("Enter port"));
        port_->setFocus();
        return;
    }

    QDialog::accept();
}
