#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QPushButton;

class UserSettings : public QDialog
{
    Q_OBJECT

public:
    UserSettings(QWidget *parent = 0, Qt::WindowFlags f = 0);

private slots:
    void requestUsers();
    void addUser();
    void removeUser();

private:
    void retranslateUi();

    QListWidget *list_;
    QPushButton *addUserButton_;
    QPushButton *removeUserButton_;
};

#endif // USERSETTINGS_H
