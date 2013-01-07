
#include <QIntValidator>
#include <QMessageBox>
#include <QTimer>

#include "commonsettingsdialog.h"
#include "ui_commonsettingsdialog.h"
#include "commonfunctions.h"
#include "database/query/settingsquery.h"

// exception
class Exception
{
public:
    Exception(const QString& error, QWidget *focusWidget = 0)
        : error_(error)
        , focusWidget_(focusWidget)
    {
    }

    const QString& what() const
    {
        return error_;
    }

    QWidget* widget() const
    {
        return focusWidget_;
    }

private:
    QString error_;
    QWidget *focusWidget_;
};
///////////////////////////////////////////////////////////////////

CommonSettingsDialog::CommonSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommonSettingsDialog)
{
    ui->setupUi(this);

    ui->lePort->setValidator(new QIntValidator(0, 99999, this));
    ui->cbStartupMode->addItems(QStringList() << QLatin1String("Automatic") << QLatin1String("Manual"));
    ui->cbTrayIcon->addItems(QStringList() << QLatin1String("Show") << QLatin1String("Hide"));

    // request settings from database
    QTimer::singleShot(0, this, SLOT(requestSettings()));
}

CommonSettingsDialog::~CommonSettingsDialog()
{
    delete ui;
}

void CommonSettingsDialog::requestSettings()
{
    database::databasequery::GetSettingsQuery query;
    query.execute();

    if (query.port() == DEFAULT_PORT)
    {
        ui->cbPort->setChecked(true);
    }
    else
    {
        ui->cbPort->setChecked(false);
        ui->lePort->setText(QString::number(query.port()));
    }

    if (query.startupMode())
    {
        ui->cbStartupMode->setCurrentIndex(0);
    }
    else
    {
        ui->cbStartupMode->setCurrentIndex(1);
    }

    if (query.trayIcon())
    {
        ui->cbTrayIcon->setCurrentIndex(0);
    }
    else
    {
        ui->cbTrayIcon->setCurrentIndex(1);
    }
}

void CommonSettingsDialog::on_cbPort_toggled(bool checked)
{
    ui->lePort->setEnabled(!checked);

    if (checked)
    {
        ui->lePort->setText(QString::number(DEFAULT_PORT));
    }
}

void CommonSettingsDialog::on_cbTrayIcon_currentIndexChanged(int index)
{
    if (index == 1)
    {
        QMessageBox::warning(this, QString(), tr("If you hide tray icon you will be able to change settings only in client programm"));
    }
}

void CommonSettingsDialog::accept()
{
    try
    {
        if (ui->lePort->text().isEmpty())
        {
            throw Exception(tr("Empty port"), ui->lePort);
        }
    }
    catch (const Exception& e)
    {
        QMessageBox::information(this, tr("Input error"), e.what());

        QWidget *widget = e.widget();
        if (widget)
        {
            widget->setFocus();
        }

        return;
    }

    // save settings to database
    int port = ui->lePort->text().toInt();
    bool trayIcon = ui->cbTrayIcon->currentIndex() == 0;
    bool startupMode = ui->cbStartupMode->currentIndex() == 0;
    database::databasequery::StoreSettingsQuery query(port, trayIcon, startupMode);
    query.execute();

    // change startupMode
    checkAutorunSettings(startupMode);

    // acception dialog
    QDialog::accept();
}
