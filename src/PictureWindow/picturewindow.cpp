
#include <QtPlugin>
#include <QLabel>
#include <QMessageBox>
#include <QIntValidator>
#include <QFileDialog>
#include <QDir>

#include "picturewindow.h"
#include "ui_picturewindow.h"
#include "pictureswidget.h"

using namespace global::params;

PluginExample::PluginExample(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PluginExample())
{   
    // save picture action (used in scroll area on context menu)
    savePictureAction_ = new QAction(this);
    connect(savePictureAction_, SIGNAL(triggered()), this, SLOT(on_savePictureButton_clicked()));

    ui->setupUi(this);

    ui->scrollArea->setWidget(new QLabel(this));
    ui->scrollArea->addAction(savePictureAction_);
    ui->scrollArea->setContextMenuPolicy(Qt::ActionsContextMenu);

    picturesWidget_ = new PicturesWidget(this);
    ui->picturesWidget->setWidget(picturesWidget_);
    ui->picturesWidget->setMinimumHeight(picturesWidget_->height());
    ui->picturesWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //ui->picturesWidget->hide();

    ui->dtFirstDate->setEnabled(false);
    ui->dtFirstDate->setDateTime(QDateTime::currentDateTime());
    ui->dtSecondDate->setEnabled(false);
    ui->dtSecondDate->setDateTime(QDateTime::currentDateTime());

    connect(ui->rbAll, SIGNAL(toggled(bool)), this, SLOT(historyAction(bool)));
    connect(ui->rbCurrent, SIGNAL(toggled(bool)), this, SLOT(historyAction(bool)));
    connect(ui->rbPeriod, SIGNAL(toggled(bool)), this, SLOT(historyAction(bool)));

    ui->cbPictureSize->setValidator(new QIntValidator(10, 3000, ui->cbPictureSize));
    ui->cbPictureSize->addItems(QStringList() << QString::number(10) << QString::number(50)
                                << QString::number(100) << QString::number(200) << QString::number(300));
    ui->cbPictureSize->setCurrentIndex(2);

    connect(ui->cbPictureSize, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangePictureSize(int)));
    connect(picturesWidget_, SIGNAL(changePicture(const QPixmap&)), this, SLOT(changePicture(const QPixmap&)));

    /* retranslte all visible strings */
    retranslateUi();
}

PluginExample::~PluginExample()
{
    delete ui;
}

QIcon PluginExample::icon() const
{
    return QIcon(":/icons/resources/icons/camera_32x32.png");
}

QString PluginExample::text() const
{
    return tr("Picture window");
}

QString PluginExample::statusTip() const
{
    return tr("Picture window");
}


void PluginExample::retranslateUi()
{
    savePictureAction_->setText(tr("Save picture"));

    ui->retranslateUi(this);
}

void PluginExample::on_takePictureButton_clicked()
{
    getClientSocket().getPicture();
}

void PluginExample::on_clearPicturesButton_clicked()
{
    picturesWidget_->clearPictures();

    QLabel *label = static_cast<QLabel*>(ui->scrollArea->widget());
    label->setPixmap(QPixmap());
}

void PluginExample::on_savePictureButton_clicked()
{
    static QDir userHomePath = QDir::home();
    QString filename = QFileDialog::getSaveFileName(this, QApplication::applicationName(), userHomePath.absolutePath(), QLatin1String("png (*.png);; jpg (*.jpg)"));

    if (!filename.isEmpty())
    {
        try
        {
            //QPixmap pixmap = picturesWidget_->getCurrentPicture();
            QLabel *label = static_cast<QLabel*>(ui->scrollArea->widget());
            QPixmap pixmap = QPixmap(*(label->pixmap()));
            bool ok = pixmap.save(filename);
            Q_ASSERT(ok);

            userHomePath.setPath(filename);
        }
        catch (std::exception&)
        {
            // ther's no picture at that moment
            return;
        }
    }
}

void PluginExample::on_historyButton_clicked()
{
    if (ui->rbAll->isChecked())
    {
        getClientSocket().getPictures();
    }
    else if (ui->rbCurrent->isChecked())
    {
        getClientSocket().getPictures(ui->dtFirstDate->dateTime());
    }
    else if (ui->rbPeriod->isChecked())
    {
        getClientSocket().getPictures(ui->dtFirstDate->dateTime(), ui->dtSecondDate->dateTime());
    }
}

void PluginExample::historyAction(bool check)
{
    if (check)
    {
        bool first;
        bool second;
        QObject *obj = sender();

        if (obj == ui->rbAll)
        {
            first = false;
            second = false;
        }
        else if (obj == ui->rbCurrent)
        {
            first = true;
            second = false;
        }
        else if (obj == ui->rbPeriod)
        {
            first = true;
            second = true;
        }

        ui->dtFirstDate->setEnabled(first);
        ui->dtSecondDate->setEnabled(second);
    }
}

void PluginExample::changePicture(const QPixmap &pixmap)
{
    //QLabel *label = static_cast<QLabel*>(ui->scrollArea->widget());
    //label->setPixmap(pixmap);
    setPicture(pixmap);
}

void PluginExample::onChangePictureSize(int index)
{
    try
    {
        const QPixmap& currentPicture = picturesWidget_->getCurrentPicture();

        setPicture(currentPicture);
    }
    catch (std::exception&)
    {
        // ther's no picture at that moment
        return;
    }
}

void PluginExample::setPicture(const QPixmap& pixmap)
{
    QLabel *label = static_cast<QLabel*>(ui->scrollArea->widget());

    int scaleInt = ui->cbPictureSize->itemText(ui->cbPictureSize->currentIndex()).toInt();
    if (scaleInt == 100)
    {
        label->setPixmap(pixmap);
    }
    else
    {
        double scaleFactor = scaleInt / 100.0;
        QSize normalPictureSize = pixmap.size();
        QSize newPictureSize = normalPictureSize * scaleFactor;
        QPixmap pictureWithNewSize = pixmap.scaled(newPictureSize, Qt::KeepAspectRatio);
        label->setPixmap(pictureWithNewSize);
    }
}

void PluginExample::on_pbSizeWidth_clicked()
{
    try
    {
        const QPixmap& currentPicture = picturesWidget_->getCurrentPicture();

        QSize scrollSize = ui->scrollArea->size();
        QSize pictureSize = currentPicture.size();
        double heightK = (double)scrollSize.height() / (double)pictureSize.height();
        double widthK = (double)scrollSize.width() / (double)pictureSize.width();
        int scaleFactor = qMin(heightK, widthK) * 100;

        int index = ui->cbPictureSize->findText(QString::number(scaleFactor));
        if (index == -1)
        {
            ui->cbPictureSize->addItem(QString::number(scaleFactor));
            index = ui->cbPictureSize->count() - 1;
        }
        ui->cbPictureSize->setCurrentIndex(index);

        setPicture(currentPicture);
    }
    catch (std::exception&)
    {
        // ther's no picture at that moment
        return;
    }
}

void PluginExample::on_pbSizeOneToOne_clicked()
{
    try
    {
        const QPixmap& currentPicture = picturesWidget_->getCurrentPicture();

        int index = ui->cbPictureSize->findText(QString::number(100));
        if (index == -1)
        {
            ui->cbPictureSize->addItem(QString::number(100));
            index = ui->cbPictureSize->count() - 1;
        }
        ui->cbPictureSize->setCurrentIndex(index);

        setPicture(currentPicture);
    }
    catch (std::exception&)
    {
        // ther's no picture at that moment
        return;
    }
}

void PluginExample::on_pbSettings_clicked()
{
    clientsocket::IClientSocket& socket = getClientSocket();

    if (socket.isConnected())
    {
        // ask mail properties from server in async mode
        socket.getMailProperties();
        // ask picture timer from server in async mode
        socket.getPictureTimer();

        PropertyDialog propertyDialog;
        connect(this, SIGNAL(setPictureInterval(const int)), &propertyDialog, SLOT(setPictureInterval(const int)));
        connect(this, SIGNAL(setMailProperties(const QString&, int, const QString&, const QString&,
            const QString&, const QStringList&, int)), 
            &propertyDialog, SLOT(setMailProperties(const QString&, int, const QString&, const QString&,
            const QString&, const QStringList&, int)));

        if (propertyDialog.exec() == QDialog::Accepted)
        {
            socket.pictureTimer(propertyDialog.picturesInterval());

            int interval;
            if (propertyDialog.changeMailSettings())
            {
                interval = propertyDialog.mailInterval();
            }
            else
            {
                // release timer
                interval = 0;
            }

            socket.mailProperties(propertyDialog.server(), propertyDialog.serverPort(),
                                             propertyDialog.user(), propertyDialog.password(),
                                             propertyDialog.sendFrom(), propertyDialog.sendTo(),
                                             interval);
        }
    }
    else
    {
        // maybe message "socket is not connected"
    }
}

void PluginExample::readData(const AbstractData &data)
{
    switch (data.type())
    {
    case GETPICTURE:
    {
        const PictureData& pictureData = static_cast<const PictureData&>(data);

        //QLabel *label = static_cast<QLabel*>(ui->scrollArea->widget());
        //label->setPixmap(pictureData.pixmap());
        setPicture(pictureData.pixmap());
        picturesWidget_->addPicture(const_cast<QPixmap&>(pictureData.pixmap()), pictureData.date().toString(QLatin1String("yyyy-MM-dd hh:mm:ss")));
    }
        break;

    case GETMAILPROPERTIES:
    {
        const MailPropertiesData& mpd = static_cast<const MailPropertiesData&>(data);
        emit setMailProperties(mpd.server_, mpd.serverPort_, mpd.user_, mpd.password_, mpd.sendFrom_, mpd.sendTo_, mpd.interval_);
    }
        break;

    /*case EXECUTIONREPORT:
    {
        const ExecutionReportData& er = static_cast<const ExecutionReportData&>(data);
        QMessageBox::information(this, tr("Server error"), er.error());
    }
        break;
        */

    case GETPICTURETIMER:
    {
        const PicturePropertiesData& pp = static_cast<const PicturePropertiesData&>(data);
        emit setPictureInterval(pp.interval());
    }
        break;

    default:
        break;
    }
}
