#ifndef DIALOG_H
#define DIALOG_H

#include <map>

#include <QMainWindow>
#include <QTcpSocket>
#include <QStringList>
#include <QTranslator>
#include <QVector>
#include <QMap>

#include "basewidget.h"
#include "clienttcpsocket.h"

class QAction;
class QVariant;
class QMenu;
class QTreeWidget;
class QToolBar;
class QTreeWidgetItem;
class QEvent;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual bool eventFilter(QObject *object, QEvent *event);

private slots:
    void addConnection();
    void removeConnection();
    void fullView();
    void onPluginAction(QAction* action);
    void closeTab(int index);
    // tree widget menu
    void onTreeWidgetContexMenuRequested(const QPoint& point);
    // tree widget select bold text current item
    void onTreeWidgetCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    // to synchronize current item with tree widget
    void onTabWidgetCurrentChanged(int);
    void connectToHost();
    void disconnectFromHost();
    void login();
    void checkAutorization(bool success, const QString& error);

    void changeLanguage(QAction *action);
    void changeStylesheet(QAction *action);

    void socketStateChanged(QAbstractSocket::SocketState state);

private:
    void createActions();
    void createToolBar();
    void createMenu();
    void createLanguageMenu();
    void createStyleSheetMenu();
    void retranslateUI();
    void loadPlugins();
    void populateStyleMap(QMap<QString, QString>& map, const QString& directory, const QStringList& filter = QStringList());

    clientsocket::ClientTcpSocket* getCurrentConnection();

    Ui::MainWindow *ui;

    std::map<int, clientsocket::ClientTcpSocket*> clientSockets_;
    std::vector<BaseWidget*> plugins_;
    std::pair<int, QWidget*> fullViewPair_;

    QString host_;
    int port_;

    // main window actions
    QAction *addConnectionAction_;
    QAction *removeConnectionAction_;
    QAction *fullViewAction_;
    QAction *defaultStyleAction_; // default style(mean no style) in menu

    // actions to tree widget context menu requested
    QAction *connectAction_;
    QAction *disconnectAction_;

    QMenu *connectMenu_;
    QMenu *languageMenu_;
    QMenu *stylesheetMenu_;
    QMenu *pluginsMenu_;

    QToolBar *toolBar_;

    QTranslator appTranslator_;
    QTranslator qtTranslator_;
    QVector<QTranslator*> pluginsTranslator_;
};

#endif // DIALOG_H
