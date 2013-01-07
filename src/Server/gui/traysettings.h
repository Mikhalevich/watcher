#ifndef TRAYSETTINGS_H
#define TRAYSETTINGS_H

#include <QSystemTrayIcon>
#include <QMenu>

class QAction;

class TraySettings : public QSystemTrayIcon
{
    Q_OBJECT

public:
    TraySettings();

private slots:
    //void onUserAction();
    void onSettingsAction();

private:
    void createActions();
    void createMenu();
    void retranslateUi();

    QMenu menu_;
    QAction* userAction_;
    QAction* settingsAction_;
    QAction* quitAction_;
};

#endif // TRAYSETTINGS_H
