
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>

#include "settingsdialog.h"
#include "usersettings.h"
#include "commonsettingsdialog.h"
#include "adduserdialog.h"
#include "autorizationdialog.h"
#include "database/query/userquery.h"
#include "database/query/autorizationquery.h"

SettingsDialog::SettingsDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    // buttons
    usersButton_ = new QPushButton(this);
    settingsButton_ = new QPushButton(this);
    exitButton_ = new QPushButton(this);

    QVBoxLayout *buttonsLayout = new QVBoxLayout(this);
    buttonsLayout->addWidget(usersButton_);
    buttonsLayout->addWidget(settingsButton_);
    buttonsLayout->addWidget(exitButton_);
    QWidget *buttonsWidget = new QWidget(this);
    buttonsWidget->setLayout(buttonsLayout);

    // labels
    aboutProgrammLabel_ = new QLabel(this);
    /*QVBoxLayout *labelsLayout = new QVBoxLayout(this);
    labelsLayout->addWidget(aboutProgrammLabel_);
    QWidget *labelsWidget = new QWidget(this);
    labelsWidget->setLayout(labelsLayout);
    */

    // layout
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(aboutProgrammLabel_);
    mainLayout->addWidget(buttonsWidget);
    setLayout(mainLayout);

    // connections
    connect(usersButton_, SIGNAL(clicked()), this, SLOT(onUsersClicked()));
    connect(settingsButton_, SIGNAL(clicked()), this, SLOT(onSettingsClicked()));
    connect(exitButton_, SIGNAL(clicked()), this, SLOT(accept()));

    // retranslate
    retranslateUi();

    // check if exists at last one user in database
    QTimer::singleShot(0, this, SLOT(checkUsers()));
}

void SettingsDialog::retranslateUi()
{
    usersButton_->setText("Users settings...");
    settingsButton_->setText("Common settings...");
    exitButton_->setText("Exit");

    QString aboutProgrammText;
    aboutProgrammText.append(QCoreApplication::instance()->applicationName());
    aboutProgrammText.append(QLatin1Char(0xAE));
    aboutProgrammText.append(tr(" Server "));
    aboutProgrammText.append(QLatin1String("v.") + QCoreApplication::instance()->applicationVersion());
    aboutProgrammText.append(tr(" for"));
#if defined Q_OS_WIN
    aboutProgrammText.append(QLatin1String(" Windows 7/Vista/XP"));
#elif defined Q_OS_LINUX
    aboutProgrammText.append(QLatin1String(" Linux"));
#endif
    aboutProgrammText.append(QLatin1String("\n\n"));
    /*aboutProgrammText.append(QLatin1String("Copyright "));
    aboutProgrammText.append(QLatin1Char(0xA9));
    aboutProgrammText.append(tr(" Michael Mihalevich. All rights not reserved"));
    */
    aboutProgrammText.append(tr("Michael Mihalevich"));

    aboutProgrammLabel_->setText(aboutProgrammText);
}

void SettingsDialog::checkUsers()
{
    // request user data from database
    database::databasequery::GetUserQuery querry;
    querry.execute();
    const database::databasequery::GetUserQuery::Conteiner& users = querry.users();

    if (users.isEmpty())
    {
        AddUserDialog dialog;
        if (dialog.exec() == QDialog::Accepted)
        {
            const QString user = dialog.user();
            const QByteArray password = dialog.password();

            // request to save user to database
            database::databasequery::StoreUserQuery querry(user, password);
            querry.execute();
            // check if querry complete successfuly
            if (querry.error().isEmpty())
            {
                QMessageBox::information(this, QString(), tr("User added successfully"));
            }
            else
            {
                QMessageBox::warning(this, tr("error"), querry.error());
            }
        }
        else
        {
            QMessageBox::information(this, tr("error"), tr("Please add at least one user"));
        }
    }
    else
    {
        bool pass = false;
        // endless cycle
        while (true)
        {
            AutorizationDialog dialog;
            if (dialog.exec() == QDialog::Accepted)
            {
                // check user
                database::databasequery::AutorizationQuery querry(dialog.user(), dialog.password());
                querry.execute();
                const QString& error = querry.error();
                if (error.isEmpty())
                {
                    // pass autorization
                    pass = true;
                    break;
                }
                else
                {
                    QMessageBox::warning(this, QString(), error);
                }
            }
            else
            {
                pass = false;
                break;
            }
        }

        if (!pass)
        {
            accept();
        }
    }
}

void SettingsDialog::onUsersClicked()
{
    UserSettings dialog;
    dialog.exec();
}

void SettingsDialog::onSettingsClicked()
{
    CommonSettingsDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        // some work, if you need
    }
}
