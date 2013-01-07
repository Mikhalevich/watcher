
#include <QIntValidator>
#include <QMessageBox>

#include "propertydialog.h"
#include "ui_propertydialog.h"

// exception
class Exception
{
public:
    Exception(const QString& error)
        : error_(error)
    {
    }

    const QString& what() const
    {
        return error_;
    }

private:
    QString error_;
};
///////////////////////////////////////////////////////////////////

PropertyDialog::PropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertyDialog),
    picturesInterval_(0)
{
    ui->setupUi(this);

    ui->mailServerPort->setValidator(new QIntValidator(0, 99999, this));

    connect(ui->cbPicturesInterval, SIGNAL(toggled(bool)), ui->picturesInterval, SLOT(setEnabled(bool)));
}

PropertyDialog::~PropertyDialog()
{
    delete ui;
}

bool PropertyDialog::changeMailSettings() const
{
    return ui->mailGroupBox->isChecked();
}

void PropertyDialog::setMailProperties(const QString &server, int serverPort,
                                       const QString &user, const QString &password,
                                       const QString &sendFrom,
                                       const QStringList &sendTo,
                                       int interval)
{
    ui->mailServer->setText(server);
    ui->mailServerPort->setText(QString::number(serverPort));
    ui->mailUser->setText(user);
    ui->mailUserPwd->setText(password);
    ui->mailSendFrom->setText(sendFrom);
    ui->mailSendTo->setPlainText(sendTo.join(QLatin1String("\n")));
    ui->mailInterval->setValue(interval);

    if (interval == 0)
    {
        ui->mailGroupBox->setChecked(false);
    }
}

void PropertyDialog::setPictureInterval(const int interval)
{
    ui->picturesInterval->setValue(interval);

    if (interval == 0)
    {
        ui->cbPicturesInterval->setChecked(false);
    }
}

void PropertyDialog::accept()
{
    // picture interval settings
    if (ui->cbPicturesInterval->isChecked())
    {
        if (ui->picturesInterval->value() < 0)
        {
            // imposible
            return;
        }

        picturesInterval_ = ui->picturesInterval->value();
    }
    else
    {
        picturesInterval_ = 0;
    }

    // mail settings
    if (ui->mailGroupBox->isChecked())
    {
        try
        {
            if (ui->mailServer->text().isEmpty())
            {
                throw Exception(tr("Enter server"));
            }

            if (ui->mailServerPort->text().isEmpty())
            {
                throw Exception(tr("Enter server port"));
            }

            if (ui->mailUser->text().isEmpty())
            {
                throw Exception(tr("Enter user"));
            }

            if (ui->mailUserPwd->text().isEmpty())
            {
                throw Exception(tr("Enter password"));
            }

            if (ui->mailSendFrom->text().isEmpty())
            {
                throw Exception(tr("Enter output email address"));
            }

            sendTo_ = ui->mailSendTo->toPlainText().split(QRegExp(QLatin1String("[\\s\\,]+")), QString::SkipEmptyParts);
            if (sendTo_.isEmpty())
            {
                throw Exception(tr("Enter recepients email addresses"));
            }

            if (ui->mailInterval->value() < 0)
            {
                throw Exception(tr("Invalid interval"));
            }
        }
        catch(Exception& e)
        {
            QMessageBox::information(this, tr("Input error"), e.what());
            return;
        }

        server_ = ui->mailServer->text();
        serverPort_ = ui->mailServerPort->text().toInt();
        user_ = ui->mailUser->text();
        password_ = ui->mailUserPwd->text();
        sendFrom_ = ui->mailSendFrom->text();
        // sendTo_ yet contein email adresses
        mailInterval_ = ui->mailInterval->value();
    }

    QDialog::accept();
}
