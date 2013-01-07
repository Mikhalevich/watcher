
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QStyle>
#include <QMessageBox>
#include <QTimer>

#include "usersettings.h"
#include "adduserdialog.h"
#include "database/query/userquery.h"

UserSettings::UserSettings(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    list_ = new QListWidget(this);

    /*list->setIconSize(QSize(60, 60));
    for (int i = (int)QStyle::SP_TitleBarMinButton; i < (int)QStyle::SP_MediaVolumeMuted; ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(QApplication::style()->standardIcon((QStyle::StandardPixmap)i), QString::number(i), list);
    }*/

    /*QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    QPushButton *okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    */

    addUserButton_ = new QPushButton(this);
    connect(addUserButton_, SIGNAL(clicked()), this, SLOT(addUser()));

    removeUserButton_ = new QPushButton(this);
    connect(removeUserButton_, SIGNAL(clicked()), this, SLOT(removeUser()));

    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(addUserButton_);
    buttonLayout->addWidget(removeUserButton_);
    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setLayout(buttonLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(list_);
    mainLayout->addWidget(buttonWidget);
    //mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    retranslateUi();

    // request users from database
    QTimer::singleShot(0, this, SLOT(requestUsers()));
}

void UserSettings::requestUsers()
{
    // request user data from database
    database::databasequery::GetUserQuery querry;
    querry.execute();
    const database::databasequery::GetUserQuery::Conteiner& users = querry.users();

    for (database::databasequery::GetUserQuery::Conteiner::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        // add user with his name
        QListWidgetItem *item = new QListWidgetItem(it->first, list_);
        // store user name and password
        item->setData(Qt::UserRole, it->first);
        item->setData(Qt::UserRole + 1, it->second);
    }
}

void UserSettings::retranslateUi()
{
    addUserButton_->setText(tr("Add user..."));
    removeUserButton_->setText(tr("Remove user"));
}

void UserSettings::addUser()
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
            // add user with his name
            QListWidgetItem *item = new QListWidgetItem(user, list_);
            // store user name and password
            item->setData(Qt::UserRole, user);
            item->setData(Qt::UserRole + 1, password);
        }
        else
        {
            QMessageBox::information(this, tr("error"), querry.error());
        }
    }
}

void UserSettings::removeUser()
{
    QListWidgetItem *item = list_->currentItem();
    if (item)
    {
        QString userName = item->data(Qt::UserRole).toString();

        if (QMessageBox::warning(this, QString::null, tr("Delete user %1?").arg(userName),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {

            // request to delete user from database
            database::databasequery::DeleteUserQuery querry(userName);
            querry.execute();
            // check if querry complete successfuly
            if (querry.error().isEmpty())
            {
                delete item;
            }
            else
            {
                QMessageBox::information(this, tr("error"), querry.error());
            }
        }
    }
    else
    {
        QMessageBox::warning(this, QString::null, tr("You should select a user"));
    }
}
