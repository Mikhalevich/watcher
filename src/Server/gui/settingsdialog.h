#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QPushButton;
class QLabel;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

private slots:
    void checkUsers();
    void onUsersClicked();
    void onSettingsClicked();

private:
    void retranslateUi();

    QPushButton *usersButton_;
    QPushButton *settingsButton_;
    QPushButton *exitButton_;

    QLabel *aboutProgrammLabel_;
};

#endif // SETTINGSDIALOG_H
