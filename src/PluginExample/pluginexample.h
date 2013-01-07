#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>

#include "basewidget.h"

/* form name */
namespace Ui {
    class PluginExample;
}


/* change PluginExample to you class name */
class PluginExample : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)

public:

    explicit PluginExample(QWidget *parent = 0);
    ~PluginExample();

    /* create object of itself */
    virtual BaseWidget* clone() const
    {
        return new PluginExample();
    }

    /* icon to menu and toolbar */
    virtual QIcon icon() const;

    /* text to menu and toolbar */
    virtual QString text() const;

    /* tip to status bar */
    virtual QString statusTip() const;

public slots:
    /* read data incoming from server */
    virtual void readData(const AbstractData& data);

    /* internationalization */
    virtual void retranslateUi();

private:
    /* form */
    Ui::PluginExample *ui;
};

#endif // PLUGINEXAMPLE_H
