#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>

namespace Ui {
    class PropertyDialog;
}

class PropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertyDialog(QWidget *parent = 0);
    ~PropertyDialog();

    int picturesInterval() const
    {
        return picturesInterval_;
    }

    const QString& server() const
    {
        return server_;
    }

    const int serverPort() const
    {
        return serverPort_;
    }

    const QString& user() const
    {
        return user_;
    }

    const QString& password() const
    {
        return password_;
    }

    const QString& sendFrom() const
    {
        return sendFrom_;
    }

    const QStringList& sendTo() const
    {
        return sendTo_;
    }

    const int mailInterval() const
    {
        return mailInterval_;
    }

    bool changeMailSettings() const;

    void setMailProperties(const QString& server, int serverPort,
                           const QString& user, const QString& password,
                           const QString& sendFrom, const QStringList& sendTo,
                           int interval);

    void setPictureInterval(const int interval);

public slots:
    virtual void accept();

private:
    Ui::PropertyDialog *ui;

    // interval in seconds between taking snapshots
    // 0 - disable timer
    int picturesInterval_;

    // mail parameters
    QString server_;
    int serverPort_;
    QString user_;
    QString password_;
    QString sendFrom_;
    QStringList sendTo_;
    int mailInterval_;
};

#endif // PROPERTYDIALOG_H
