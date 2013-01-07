#ifndef AURORIZATIONDIALOG_H
#define AURORIZATIONDIALOG_H

#include <QDialog>

class QLineEdit;

class AurorizationDialog : public QDialog
{
    Q_OBJECT

public:
    AurorizationDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);

    QString userName() const;
    QByteArray password() const;

public slots:
    virtual void accept();

private:
    QLineEdit *userName_;
    QLineEdit *password_;
};

#endif // AURORIZATIONDIALOG_H
