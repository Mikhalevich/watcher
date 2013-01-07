#ifndef AUTORIZATIONDIALOG_H
#define AUTORIZATIONDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

class AutorizationDialog : public QDialog
{
    Q_OBJECT

public:
    AutorizationDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    const QString user() const;
    const QByteArray password() const;

public slots:
    virtual void accept();

private:
    void retranslateUi();

    QLineEdit *userName_;
    QLineEdit *password_;

    QLabel *userNameLabel_;
    QLabel *passwordLabel_;
};

#endif // AUTORIZATIONDIALOG_H
