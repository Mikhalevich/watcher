
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCryptographicHash>

#include "aurorizationdialog.h"

AurorizationDialog::AurorizationDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    //this->setAttribute(Qt::WA_DeleteOnClose);

    QLabel *userLabel = new QLabel(tr("User name"), this);
    QLabel *userPwd = new QLabel(tr("password"), this);

    userName_ = new QLineEdit(this);
    password_ = new QLineEdit(this);
    password_->setEchoMode(QLineEdit::Password);

    QDialogButtonBox *box = new QDialogButtonBox(Qt::Horizontal, this);
    QPushButton *okButton = box->addButton(QDialogButtonBox::Ok);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = box->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(userLabel, 0, 0);
    layout->addWidget(userName_, 0, 1);
    layout->addWidget(userPwd, 1, 0);
    layout->addWidget(password_, 1, 1);
    layout->addWidget(box, 2, 0, 1, 2);

    userName_->setFocus();
}

QString AurorizationDialog::userName() const
{
    return userName_->text();
}

QByteArray AurorizationDialog::password() const
{
    // return hash of the password
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(password_->text().toUtf8());
    return hash.result();
}

void AurorizationDialog::accept()
{
    if (userName_->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input error"), tr("Enter user name"));
        userName_->setFocus();
        return;
    }

    if (password_->text().isEmpty())
    {
        QMessageBox::information(this, tr("Input error"), tr("Enter password"));
        password_->setFocus();
        return;
    }

    QDialog::accept();
}

