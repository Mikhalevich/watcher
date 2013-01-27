
#include <QApplication>
#include <QStyle>
#include <QAction>

#include "traysettings.h"
#include "usersettings.h"
#include "settingsdialog.h"

TraySettings::TraySettings()
{
    //setIcon(QApplication::style()->standardIcon(QStyle::SP_DesktopIcon));
    setIcon(QIcon(":/icons/resources/icons/comment_32x32.png"));

    createActions();
    createMenu();

    retranslateUi();
}

void TraySettings::createActions()
{
    settingsAction_ = new QAction(this);
    connect(settingsAction_, SIGNAL(triggered()), this, SLOT(onSettingsAction()));

    /*userAction_ = new QAction(this);
    connect(userAction_, SIGNAL(triggered()), this, SLOT(onUserAction()));
    */

    quitAction_ = new QAction(this);
    connect(quitAction_, SIGNAL(triggered()), QCoreApplication::instance(), SLOT(quit()));
}

void TraySettings::createMenu()
{
    menu_.addAction(settingsAction_);
    //menu_.addAction(userAction_);
    menu_.addSeparator();
    menu_.addAction(quitAction_);

    setContextMenu(&menu_);
}

void TraySettings::retranslateUi()
{
    settingsAction_->setText(tr("Settings..."));
    //settingsAction_->setShortcut(tr("Ctrl+S"));

    /*userAction_->setText(tr("User settings..."));
    userAction_->setShortcut(tr("Ctrl+U"));
    */

    quitAction_->setText(tr("quit"));
    //quitAction_->setShortcut(tr("Ctrl+Q"));

    setToolTip(QCoreApplication::applicationFilePath());
}

/*void TraySettings::onUserAction()
{
    UserSettings settings;

    if (settings.exec() == QDialog::Accepted)
    {
        //some work
    }
}*/

void TraySettings::onSettingsAction()
{
    SettingsDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        // some work
    }
}
