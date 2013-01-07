
#include <QtPlugin>

#include "pluginexample.h"
#include "ui_pluginexample.h"

PluginExample::PluginExample(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::PluginExample())
{   
    /* retranslte all visible strings */
    retranslateUi();
}

PluginExample::~PluginExample()
{
    delete ui;
}

QIcon PluginExample::icon() const
{
    return QIcon();
}

QString PluginExample::text() const
{
    return tr("Plugin example tool tip");
}

QString PluginExample::statusTip() const
{
    return tr("Plugin example status tip");
}


void PluginExample::retranslateUi()
{
}


void PluginExample::readData(const AbstractData &data)
{
    switch (data.type())
    {
    case GETALLWINDOWS:
    {
        const WindowData& windowData = static_cast<const WindowData&>(data);
    }
        break;

    case GETALLCONTROLS:
    {
        const ControlData& controlData = static_cast<const ControlData&>(data);
    }
        break;

    case GETCONTROLINFO:
    {
        const ControlInfoData& controlInfoData = static_cast<const ControlInfoData&>(data);
    }
        break;

    default:
        Q_ASSERT("You should never be here" && false);
    }
}

/* export plugin */
/* first parameter plugin name - see TARGET in .pro file */
/* second parameter plugin class - see class name in .h file */
Q_EXPORT_PLUGIN2(PluginExample, PluginExample)
