#ifndef ACTIONWINDOW_H
#define ACTIONWINDOW_H

#include <QWidget>

#include "basewidget.h"

namespace Ui {
    class ActionWindow;
}

class ActionWindow : public BaseWidget
{
    Q_OBJECT
    Q_INTERFACES(BaseWidget)

public:
    explicit ActionWindow(QWidget *parent = 0);
    ~ActionWindow();

    virtual BaseWidget* clone() const
    {
        return new ActionWindow();
    }

    virtual QIcon icon() const;
    virtual QString text() const;
    virtual QString statusTip() const;

private slots:
    void on_allWindowsButton_clicked();
    void on_controlsButton_clicked();
    void on_textButton_clicked();
    void on_infoButton_clicked();
    void on_processButton_clicked();

    virtual void readData(const AbstractData& data);
    virtual void retranslateUi();

private:
    Ui::ActionWindow *ui;
};

#endif // ACTIONWINDOW_H
