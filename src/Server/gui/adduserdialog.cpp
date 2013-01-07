
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QCryptographicHash>

#include "adduserdialog.h"

// exception
class Exception
{
public:
    Exception(const QString& error, QWidget *focusWidget = 0)
        : error_(error)
        , focusWidget_(focusWidget)
    {
    }

    const QString& what() const
    {
        return error_;
    }

    QWidget* widget() const
    {
        return focusWidget_;
    }

private:
    QString error_;
    QWidget *focusWidget_;
};
///////////////////////////////////////////////////////////////////

AddUserDialog::AddUserDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


    userNameLabel_ = new QLabel(this);
    passwordLabel_ = new QLabel(this);
    repeatPasswordLabel_ = new QLabel(this);

    userName_ = new QLineEdit(this);
    password_ = new QLineEdit(this);
    password_->setEchoMode(QLineEdit::Password);
    repeatPassword_ = new QLineEdit(this);
    repeatPassword_->setEchoMode(QLineEdit::Password);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(userNameLabel_, 0, 0);
    mainLayout->addWidget(userName_, 0, 1);
    mainLayout->addWidget(passwordLabel_, 1, 0);
    mainLayout->addWidget(password_, 1, 1);
    mainLayout->addWidget(repeatPasswordLabel_, 2, 0);
    mainLayout->addWidget(repeatPassword_, 2, 1);
    mainLayout->addWidget(buttonBox, 3, 0, 1, 2);

    setLayout(mainLayout);

    userName_->setFocus();

    retranslateUi();
}

void AddUserDialog::retranslateUi()
{
    userNameLabel_->setText("User name");
    passwordLabel_->setText("Password");
    repeatPasswordLabel_->setText("Confirm password");

    setWindowTitle(tr("Add user"));
}

const QString AddUserDialog::user() const
{
    return userName_->text();
}

const QByteArray AddUserDialog::password() const
{
    // return hash of the password
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(password_->text().toUtf8());
    return hash.result();
}

void AddUserDialog::accept()
{
    try
    {
        if (userName_->text().isEmpty())
        {
            throw Exception(tr("Enter user name"), userName_);
        }

        if (password_->text().isEmpty())
        {
            throw Exception(tr("You must enter password"), password_);
        }

        if (repeatPassword_->text().isEmpty())
        {
            throw Exception(tr("You must repeat password"), repeatPassword_);
        }

        if (password_->text() != repeatPassword_->text())
        {
            throw Exception(tr("Entered passwords do not match"), password_);
        }
    }
    catch (const Exception& e)
    {
        QMessageBox::information(this, tr("Input error"), e.what());

        QWidget *widget = e.widget();
        if (widget)
        {
            widget->setFocus();
        }

        return;
    }

    QDialog::accept();
}
