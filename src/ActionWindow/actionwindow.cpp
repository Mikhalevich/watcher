
#include <QMessageBox>
#include <QtPlugin>

#include "actionwindow.h"
#include "ui_actionwindow.h"

ActionWindow::ActionWindow(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ActionWindow)
{
    ui->setupUi(this);

    ui->windowTableWidget->setColumnCount(2);
    ui->windowTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->windowTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->controlsTableWidget->setColumnCount(2);
    ui->controlsTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->controlsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->rbEdit->setChecked(true);

    retranslateUi();
}

ActionWindow::~ActionWindow()
{
    delete ui;
}

QIcon ActionWindow::icon() const
{
    return QIcon();
}

QString ActionWindow::text() const
{
    return tr("Action tool tip");
}

QString ActionWindow::statusTip() const
{
    return tr("Action status tip");
}


void ActionWindow::retranslateUi()
{
    ui->windowTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Class"));
    ui->controlsTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Name") << tr("Class"));
}

void ActionWindow::on_allWindowsButton_clicked()
{
    ui->windowTableWidget->setRowCount(0);
    getClientSocket().getAllWindows();
}

void ActionWindow::on_controlsButton_clicked()
{
    ui->controlsTableWidget->setRowCount(0);
    int currentRow = ui->windowTableWidget->currentRow();
    QTableWidgetItem *item = ui->windowTableWidget->item(currentRow, 0);

    if (item)
    {
        getClientSocket().getAllControls(item->text());
    }
    else
    {
        QMessageBox::information(this, tr("Input error"), tr("You must select window name"));
    }
}

void ActionWindow::on_textButton_clicked()
{
    int currentRow = ui->controlsTableWidget->currentRow();
    QTableWidgetItem *item = ui->controlsTableWidget->item(currentRow, 0);

    if (item)
    {
        // (qint16)ui->controlsTableWidget->currentRow();
        qint16 controlIndex = (qint16) item->data(Qt::UserRole).toInt();
        qint8 controlType;

        if (ui->rbEdit->isChecked())
            controlType = (qint8)EDIT;
        else if (ui->rbButton->isChecked())
            controlType = (qint8)BUTTON;
        else if (ui->rbCombo->isChecked())
            controlType = (qint8)COMBOBOX;

        QString newText = ui->setterTextEdit->toPlainText();

        getClientSocket().setControlText(controlIndex, controlType, newText);
    }
    else
    {
        QMessageBox::information(this, tr("Input error"), tr("You must select a control"));
    }
}

void ActionWindow::on_infoButton_clicked()
{
    int currentRow = ui->controlsTableWidget->currentRow();
    QTableWidgetItem *item = ui->controlsTableWidget->item(currentRow, 0);

    if (item)
    {
        qint16 controlIndex = (qint16)item->data(Qt::UserRole).toInt();
        qint8 controlType;

        if (ui->rbEdit->isChecked())
            controlType = (qint8)EDIT;
        else if (ui->rbButton->isChecked())
            controlType = (qint8)BUTTON;
        else if (ui->rbCombo->isChecked())
        {
            // clear text edit before get data from the server
            ui->infoTextEdit->clear();
            controlType = (qint8)COMBOBOX;
        }

        getClientSocket().getControlInfo(controlIndex, controlType);
    }
    else
    {
        QMessageBox::information(this, tr("Input error"), tr("You must select a control"));
    }
}

void ActionWindow::on_processButton_clicked()
{
    if (ui->processLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("input error"), tr("Please enter process command line to execute"));
    }
    else
    {
        //operation_ = RUNPROCESS;
        //connectToServer();
        QString processLine = ui->processLineEdit->text();

        getClientSocket().runProcess(processLine);
    }
}

void ActionWindow::readData(const AbstractData &data)
{
    switch (data.type())
    {
    case GETALLWINDOWS:
    {
        const WindowData& windowData = static_cast<const WindowData&>(data);

        int row = ui->windowTableWidget->rowCount();
        ui->windowTableWidget->insertRow(row);

        QString windowName = windowData.windowName();
        QTableWidgetItem *nameItem = new QTableWidgetItem(windowName);
        ui->windowTableWidget->setItem(row, 0, nameItem);

        QString windowClass = windowData.windowClass();
        QTableWidgetItem *classItem = new QTableWidgetItem(windowClass);
        ui->windowTableWidget->setItem(row, 1, classItem);

        ui->windowTableWidget->resizeColumnsToContents();
    }
        break;

    case GETALLCONTROLS:
    {
        const ControlData& controlData = static_cast<const ControlData&>(data);

        int row = ui->controlsTableWidget->rowCount();
        ui->controlsTableWidget->insertRow(row);

        int controlIndex = controlData.index();

        QString controlText = controlData.controlText();
        QTableWidgetItem *nameItem = new QTableWidgetItem(controlText);
        nameItem->setData(Qt::UserRole, controlIndex);
        ui->controlsTableWidget->setItem(row, 0, nameItem);

        QString controlClass = controlData.controlClass();
        QTableWidgetItem *classItem = new QTableWidgetItem(controlClass);
        ui->controlsTableWidget->setItem(row, 1, classItem);

        ui->controlsTableWidget->resizeColumnsToContents();
    }
        break;

    case GETCONTROLINFO:
    {
        const ControlInfoData& controlInfoData = static_cast<const ControlInfoData&>(data);
        QString controlInfo = controlInfoData.controlInfo();
        if (ui->rbEdit->isChecked())
        {
            ui->infoTextEdit->setText(controlInfo);
        }
        else if (ui->rbButton->isChecked())
        {
            ui->infoTextEdit->setText(controlInfo);
        }
        else if (ui->rbCombo->isChecked())
        {
            ui->infoTextEdit->append(controlInfo);
        }
    }
        break;

    default:
        Q_ASSERT("You should never be here" && false);
    }
}

Q_EXPORT_PLUGIN2(ActionWindow, ActionWindow)
