#ifndef COMMONSETTINGSDIALOG_H
#define COMMONSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class CommonSettingsDialog;
}

// default port
const int DEFAULT_PORT = 7777;

class CommonSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CommonSettingsDialog(QWidget *parent = 0);
    ~CommonSettingsDialog();

public slots:
    virtual void accept();

private slots:
    void requestSettings();
    void on_cbPort_toggled(bool checked);
    void on_cbTrayIcon_currentIndexChanged(int index);

private:
    Ui::CommonSettingsDialog *ui;
};

#endif // COMMONSETTINGSDIALOG_H
