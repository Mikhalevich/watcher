#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog(QString host, int port, QWidget *parent = 0, Qt::WindowFlags f = 0);

    QString host() const
    {
        return host_->text();
    }

    int port() const
    {
        return port_->text().toInt();
    }

public slots:
    virtual void accept();

private:
    QLineEdit *host_;
    QLineEdit *port_;
};

#endif // CONNECTDIALOG_H
