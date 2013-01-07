#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    AddUserDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    const QString user() const;
    const QByteArray password() const;

public slots:
    virtual void accept();

private:
    void retranslateUi();

    QLineEdit *userName_;
    QLineEdit *password_;
    QLineEdit *repeatPassword_;

    QLabel *userNameLabel_;
    QLabel *passwordLabel_;
    QLabel *repeatPasswordLabel_;
};

#endif // ADDUSERDIALOG_H
