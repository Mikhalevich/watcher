
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QCryptographicHash>

#include "autorizationdialog.h"

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

AutorizationDialog::AutorizationDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


    userNameLabel_ = new QLabel(this);
    passwordLabel_ = new QLabel(this);

    userName_ = new QLineEdit(this);
    password_ = new QLineEdit(this);
    password_->setEchoMode(QLineEdit::Password);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(userNameLabel_, 0, 0);
    mainLayout->addWidget(userName_, 0, 1);
    mainLayout->addWidget(passwordLabel_, 1, 0);
    mainLayout->addWidget(password_, 1, 1);
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2);

    setLayout(mainLayout);

    userName_->setFocus();

    retranslateUi();
}

void AutorizationDialog::retranslateUi()
{
    userNameLabel_->setText("User name");
    passwordLabel_->setText("Password");

    setWindowTitle(tr("Autorization"));
}

const QString AutorizationDialog::user() const
{
    return userName_->text();
}

const QByteArray AutorizationDialog::password() const
{
    // return hash of the password
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(password_->text().toUtf8());
    return hash.result();
}

void AutorizationDialog::accept()
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
