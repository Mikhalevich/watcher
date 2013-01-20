
#include <QtPlugin>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTimer>

#include "settingswindow.h"

namespace 
{
    const int DEFAULT_PORT = 7777;

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
}

class SettingsWindowImpl
{
public:
    // common settings
    QLabel *labelPort;
    QLabel *labelSturtupMode;
    QLabel *labelTrayIcon;
    QLineEdit *lePort;
    QCheckBox *cbDefaultPort;
    QComboBox *comboStartupMode;
    QComboBox *comboTrayIcon;

    // common ok and cancel buttons
    QDialogButtonBox *buttonBox;
};

SettingsWindow::SettingsWindow(QWidget *parent) 
    : BaseWidget(parent)
    , impl_(new SettingsWindowImpl())
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGridLayout *commonSettingLayout = new QGridLayout(this);

    impl_->labelPort = new QLabel(this);
    impl_->lePort = new QLineEdit(this);
    impl_->lePort->setValidator(new QIntValidator(0, 99999, this));
    impl_->cbDefaultPort = new QCheckBox(this);
    commonSettingLayout->addWidget(impl_->labelPort, 0, 0);
    commonSettingLayout->addWidget(impl_->lePort, 0, 1);
    commonSettingLayout->addWidget(impl_->cbDefaultPort, 0, 2);

    impl_->labelSturtupMode = new QLabel(this);
    impl_->comboStartupMode = new QComboBox(this);
    commonSettingLayout->addWidget(impl_->labelSturtupMode, 1, 0);
    commonSettingLayout->addWidget(impl_->comboStartupMode, 1, 1, 1, 2);

    impl_->labelTrayIcon = new QLabel(this);
    impl_->comboTrayIcon = new QComboBox(this);
    commonSettingLayout->addWidget(impl_->labelTrayIcon, 2, 0);
    commonSettingLayout->addWidget(impl_->comboTrayIcon, 2, 1, 1, 2);

    QWidget *commonSettingsWidget = new QWidget(this);
    commonSettingsWidget->setLayout(commonSettingLayout);

    mainLayout->addWidget(commonSettingsWidget);
    mainLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    impl_->buttonBox = new QDialogButtonBox(this);
    impl_->buttonBox->addButton(QDialogButtonBox::Ok);
    impl_->buttonBox->addButton(QDialogButtonBox::Cancel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(impl_->buttonBox);
    QWidget *buttonWidget = new QWidget(this);
    buttonWidget->setLayout(buttonLayout);

    mainLayout->addWidget(buttonWidget);
    setLayout(mainLayout);

    createConnections();

    /* retranslate all visible strings */
    retranslateUi();
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::showEvent(QShowEvent *event)
{
    readSettings();
    BaseWidget::showEvent(event);
}

QIcon SettingsWindow::icon() const
{
    return QIcon(":/icons/resources/icons/gear_wheel_32x32.png");
}

QString SettingsWindow::text() const
{
    return tr("Settings Window");
}

QString SettingsWindow::statusTip() const
{
    return tr("Settings Window");
}

void SettingsWindow::retranslateUi()
{
    impl_->labelPort->setText(tr("Port"));
    impl_->labelSturtupMode->setText(tr("Startup mode"));
    impl_->labelTrayIcon->setText(tr("Tray icon"));
    impl_->cbDefaultPort->setText(tr("Default"));

    impl_->comboStartupMode->addItems(QStringList() << QLatin1String("Automatic") << QLatin1String("Manual"));
    impl_->comboTrayIcon->addItems(QStringList() << QLatin1String("Show") << QLatin1String("Hide"));
}

void SettingsWindow::createConnections()
{
    connect(impl_->cbDefaultPort, SIGNAL(toggled(bool)), this, SLOT(onDefaultPortChecked(bool)));
    connect(impl_->buttonBox, SIGNAL(accepted()), this, SLOT(onAcceptSettings()));
}

void SettingsWindow::onDefaultPortChecked(bool checked)
{
    impl_->lePort->setEnabled(!checked);

    if (checked)
    {
        impl_->lePort->setText(QString::number(DEFAULT_PORT));
    }
}

void SettingsWindow::readSettings()
{
    if (getClientSocket().isConnected())
    {
        getClientSocket().getSettings();
    }
}

void SettingsWindow::onAcceptSettings()
{
    try
    {
        if (impl_->lePort->text().isEmpty())
        {
            throw Exception(tr("Empty port"), impl_->lePort);
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
    qint32 port = impl_->lePort->text().toInt();
    // 1 - automatic, 0 - manual
    qint8 startupMode = impl_->comboStartupMode->currentIndex() == 0 ? 1 : 0;
    // 1 - show, 0 - hide
    qint8 trayIcon = impl_->comboTrayIcon->currentIndex() == 0 ? 1 : 0;

    if (getClientSocket().isConnected())
    {
        getClientSocket().setSettings(port, startupMode, trayIcon);
    }
}

void SettingsWindow::populateSettings(const SettingsData& settings)
{
    impl_->lePort->setText(QString::number(settings.port()));

    int index = -1;
    if (settings.startupMode() == 1)
    {
        index = 0;
    }
    else
    {
        index = 1;
    }
    impl_->comboStartupMode->setCurrentIndex(index);

    if (settings.trayIcon() == 1)
    {
        index = 0;
    }
    else
    {
        index = 1;
    }
    impl_->comboTrayIcon->setCurrentIndex(index);
}

void SettingsWindow::readData(const AbstractData &data)
{
    if (data.type() == GETSETTINGS)
    {
        const SettingsData& settingsData = static_cast<const SettingsData&>(data);

        populateSettings(settingsData);
    }
}

/* export plugin */
/* first parameter plugin name - see TARGET in .pro file */
/* second parameter plugin class - see class name in .h file */
Q_EXPORT_PLUGIN2(SettingsWindow, SettingsWindow)
