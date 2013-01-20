#ifndef PLUGINEXAMPLE_H
#define PLUGINEXAMPLE_H

#include <QWidget>

#include "basewidget.h"

class SettingsWindowImpl;

/* change PluginExample to you class name */
class SettingsWindow : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)

public:

    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

    /* create object of itself */
    virtual BaseWidget* clone() const
    {
        return new SettingsWindow();
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

protected:
    void showEvent(QShowEvent * event);

private slots:
    void onDefaultPortChecked(bool checked);
    void onAcceptSettings();
    void readSettings();

private:
    void createConnections();
    void populateSettings(const SettingsData& settings);

    QScopedPointer<SettingsWindowImpl> impl_;
};

#endif // PLUGINEXAMPLE_H
