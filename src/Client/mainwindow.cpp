
#include <iostream>

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QActionGroup>
#include <QDir>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPluginLoader>
#include <QToolBar>
#include <QEvent>
#include <QKeyEvent>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "aurorizationdialog.h"

namespace clientCommon
{
    const QString g_languagePath("../language");
    const QString g_pluginPath("../plugins");

    const char* g_defaultLanguage = "English";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    host_(QLatin1String("127.0.0.1")),
    port_(7777)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(2);
    ui->treeWidget->header()->setVisible(true);
    ui->treeWidget->setHeaderLabels(QStringList() << tr("Connection name") << tr("State"));

    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->setTabShape(QTabWidget::Triangular);

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    createActions();
    createToolBar();
    createMenu();
    loadPlugins();

    qApp->installTranslator(&appTranslator_);
    qApp->installTranslator(&qtTranslator_);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    // menu
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onTreeWidgetContexMenuRequested(const QPoint&)));
    // select current item
    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(onTreeWidgetCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    // synchronize current item with tree widget
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabWidgetCurrentChanged(int)));

    retranslateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPlugins()
{
    // make plugin path
    QString path = qApp->applicationDirPath() + QDir::separator() + clientCommon::g_pluginPath;
    QDir pluginDirectory(path);

    // set only files (it mean *.so on Unix, *.dynlib on MacOsX, .dll on Windows)
    QStringList fileNames = pluginDirectory.entryList(QDir::Files);

    foreach(QString file, fileNames)
    {
            QPluginLoader pluginLoader(pluginDirectory.absoluteFilePath(file));

            // get a plugin
            QObject *plugin = pluginLoader.instance();

            if (plugin)
            {
                    // is that our interface?
                    BaseWidget *baseWidget = qobject_cast<BaseWidget *>(plugin);

                    if (baseWidget)
                    {
                            // do all job
                            plugins_.push_back(baseWidget);
                    }
            }
    }

    // add plugins to menu and toolbar
    QActionGroup *pluginActionGroup = new QActionGroup(this);
    connect(pluginActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onPluginAction(QAction*)));

    if (plugins_.size() > 0)
    {
        toolBar_->addSeparator();
    }

    for (int i = 0; i < plugins_.size(); ++i)
    {
        BaseWidget *baseWidget = plugins_.at(i);
        // add action
        //QAction *action = new QAction(baseWidget->icon(), baseWidget->text(), this);
        //action->setStatusTip(baseWidget->statusTip());
        QAction *action = new QAction(this);
        action->setIcon(baseWidget->icon());
        action->setData(i);
        pluginActionGroup->addAction(action);

        pluginsMenu_->addAction(action);
        toolBar_->addAction(action);

        // install translators for plugins
        QTranslator *pluginTranslator = new QTranslator(this);
        qApp->installTranslator(pluginTranslator);
        pluginsTranslator_.push_back(pluginTranslator);
    }
}

void MainWindow::onPluginAction(QAction *action)
{
    BaseWidget *widget = plugins_.at(action->data().toInt())->clone();

    clientsocket::ClientTcpSocket *socket = getCurrentConnection();
    if (socket)
    {
        widget->setClientSocket(getCurrentConnection());
        ui->tabWidget->addTab(widget, widget->text());
    }
}

void MainWindow::createActions()
{
    addConnectionAction_ = new QAction(this);
    connect(addConnectionAction_, SIGNAL(triggered()), this, SLOT(addConnection()));

    removeConnectionAction_ = new QAction(this);
    connect(removeConnectionAction_, SIGNAL(triggered()), this, SLOT(removeConnection()));

    fullViewAction_ = new QAction(this);
    fullViewAction_->setIcon(QIcon(":/icons/resources/icons/round_up_arrow_32x32.png"));
    connect(fullViewAction_, SIGNAL(triggered()), this, SLOT(fullView()));

    connectAction_ = new QAction(this);
    connect(connectAction_, SIGNAL(triggered()), this, SLOT(connectToHost()));

    disconnectAction_ = new QAction(this);
    connect(disconnectAction_, SIGNAL(triggered()), this, SLOT(disconnectFromHost()));

    // all connectin and processing this action in createStylesheetMenu
    defaultStyleAction_ = new QAction(this);
    defaultStyleAction_->setCheckable(true);
}

void MainWindow::createToolBar()
{
    toolBar_ = new QToolBar();
    addToolBar(toolBar_);

    toolBar_->addAction(fullViewAction_);
}

void MainWindow::createMenu()
{
    connectMenu_ = new QMenu(this);
    connectMenu_->addAction(addConnectionAction_);
    connectMenu_->addAction(removeConnectionAction_);

    createLanguageMenu();
    createStyleSheetMenu();

    pluginsMenu_ = new QMenu(this);

    menuBar()->addMenu(connectMenu_);

    if (!languageMenu_->actions().isEmpty())
    {
        menuBar()->addMenu(languageMenu_);
    }

    if (!stylesheetMenu_->actions().isEmpty())
    {
        menuBar()->addMenu(stylesheetMenu_);
    }

    menuBar()->addMenu(pluginsMenu_);
}

void MainWindow::createLanguageMenu()
{
    languageMenu_ = new QMenu(this);
    QActionGroup *actions = new QActionGroup(this);
    connect(actions, SIGNAL(triggered(QAction*)), this, SLOT(changeLanguage(QAction*)));

    QDir dir(clientCommon::g_languagePath);
    QStringList qmFiles = dir.entryList(QStringList() << QLatin1String("Client_*.qm"));

    for (QStringList::const_iterator it = qmFiles.begin(); it != qmFiles.end(); ++it)
    {
        QString locale(*it);
        // remove all text before '_'
        locale.remove(0, locale.indexOf(QLatin1Char('_')) + 1);
        // remove ".qm"
        locale.chop(3);

        QTranslator tempTranslator;
        tempTranslator.load(*it, dir.absolutePath());
        QString language = tempTranslator.translate("MenuLanguage", clientCommon::g_defaultLanguage);

        QAction *languageAction = new QAction(language, this);
        languageAction->setCheckable(true);
        languageAction->setData(locale);
        actions->addAction(languageAction);

        if (language == QLatin1String(clientCommon::g_defaultLanguage))
            languageAction->setChecked(true);
    }

    languageMenu_->addActions(actions->actions());
}

void MainWindow::changeLanguage(QAction *action)
{
    QString locale(action->data().toString());
    const QString clientFile = QLatin1String("Client_") + locale + QLatin1String(".qm");

    // translate client and Qt strings
    appTranslator_.load(clientFile, clientCommon::g_languagePath);
    qtTranslator_.load(QLatin1String("qt_") + locale + QLatin1String(".qm"), clientCommon::g_languagePath);

    QString language = appTranslator_.translate("MenuLanguage", clientCommon::g_defaultLanguage);
    if (language == QLatin1String(clientCommon::g_defaultLanguage))
    {
        for (int i = 0; i < pluginsTranslator_.size(); ++i)
        {
            pluginsTranslator_.at(i)->load(QLatin1String(""), clientCommon::g_languagePath);
        }
    }
    else
    {
        // translate plugins strings
        QDir dir(clientCommon::g_languagePath);
        QStringList qmPluginFiles = dir.entryList(QStringList() << QLatin1String("*_") + locale + QLatin1String(".qm"));

        Q_ASSERT(pluginsTranslator_.size() >= qmPluginFiles.size());

        int emptyTranslator = 0;
        for (int i = 0, size = qmPluginFiles.size(); i < size; ++i)
        {
            const QString pluginFile = qmPluginFiles.at(i);
            if (pluginFile != clientFile)
            {
                pluginsTranslator_.at(emptyTranslator++)->load(pluginFile, clientCommon::g_languagePath);
            }
        }
    }

    retranslateUI();
    ui->retranslateUi(this);
}

void MainWindow::createStyleSheetMenu()
{
    stylesheetMenu_ = new QMenu(this);

    // stylesheet path
    QString path = qApp->applicationDirPath() + QLatin1String("/../stylesheet");
    QDir stylesheetDirectory(path);
    QStringList qssFiles = QStringList() << QLatin1String("*.qss") << QLatin1String("*.css");

    QMap<QString, QString> stylesheetMap;

    QStringList directories = stylesheetDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // search stylesheet on subdirectories
    foreach (QString directory, directories)
    {
        populateStyleMap(stylesheetMap, stylesheetDirectory.absoluteFilePath(directory), qssFiles);
    }

    // search stylesheet on current derictory
    populateStyleMap(stylesheetMap, path, qssFiles);

    if (!stylesheetMap.isEmpty())
    {
        QActionGroup *qssActionGroup = new QActionGroup(this);
        qssActionGroup->setExclusive(true);
        connect(qssActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeStylesheet(QAction*)));

        qssActionGroup->addAction(defaultStyleAction_);
        defaultStyleAction_->setChecked(true);

        for (QMap<QString, QString>::iterator it = stylesheetMap.begin(); it != stylesheetMap.end(); ++it)
        {
            QAction *action = new QAction(it.key(), this);
            action->setCheckable(true);
            action->setData(it.value());
            qssActionGroup->addAction(action);
        }

        stylesheetMenu_->addActions(qssActionGroup->actions());
    }
}

void MainWindow::populateStyleMap(QMap<QString, QString>& map, const QString& directory, const QStringList& filter)
{
    QDir dir(directory);

    QStringList fileNames = dir.entryList(filter, QDir::Files);

    if (!fileNames.isEmpty())
    {
        foreach (QString file, fileNames)
        {
            QFileInfo fi(file);
            QString baseName = fi.baseName();
            QString absFileName = dir.absoluteFilePath(file);

            map.insert(baseName, absFileName);
        }
    }
}

void MainWindow::changeStylesheet(QAction *action)
{
    if (action == defaultStyleAction_)
    {
        qApp->setStyleSheet(QLatin1String(""));
    }
    else
    {
        // stylesheet path
        QString absFileName = action->data().toString();
        QFile file(absFileName);
        if (file.exists())
        {
            file.open(QIODevice::ReadOnly);
            QTextStream fileStream(&file);
            QString stylesheet = fileStream.readAll();

            // get folder path
            int lastSlash = absFileName.lastIndexOf(QLatin1Char('/'));
            absFileName.remove(lastSlash, absFileName.size() - lastSlash);

            // change url(fileName) to url(fullPath/fileName)
            absFileName.prepend(QLatin1String("url("));
            absFileName.append(QLatin1String("/\\1)"));

            QRegExp regexp(QRegExp(QLatin1String("url\\((.+)\\)")));
            regexp.setMinimal(true);
            stylesheet.replace(regexp, absFileName);

#ifdef DEBUG
            qDebug() << stylesheet;
#endif

            qApp->setStyleSheet(stylesheet);
        }
        else
        {
            //todo
        }
    }
}

void MainWindow::retranslateUI()
{
    ui->treeWidget->setHeaderLabels(QStringList() << tr("Connection name") << tr("State"));
    ui->connectionDock->setWindowTitle(tr("Connections"));

    addConnectionAction_->setText(tr("&Add new connection..."));
    addConnectionAction_->setShortcut(tr("Ctrl+A"));
    addConnectionAction_->setToolTip(tr("Add new connection"));

    removeConnectionAction_->setText(tr("&Remove connection..."));
    removeConnectionAction_->setShortcut(tr("Ctrl+R"));
    removeConnectionAction_->setToolTip(tr("Remove current connection"));

    fullViewAction_->setText(tr("Full View"));
    fullViewAction_->setShortcut(tr("Ctrl+F"));
    fullViewAction_->setToolTip(tr("Switch to full view mode"));

    connectAction_->setText(tr("&Connect to host"));
    connectAction_->setShortcut(tr("Ctrl+C"));

    disconnectAction_->setText(tr("&Disconnect from host"));
    disconnectAction_->setShortcut(tr("Ctrl+D"));

    defaultStyleAction_->setText(tr("Default"));

    connectMenu_->setTitle(tr("&File"));
    languageMenu_->setTitle(tr("&Language"));
    stylesheetMenu_->setTitle(tr("&Style"));
    pluginsMenu_->setTitle(tr("&Plugins"));

    toolBar_->setWindowTitle(tr("Toolbar"));

    // retranslate plugins
    QList<QAction*> actions = pluginsMenu_->actions();
    for (QList<QAction*>::iterator it = actions.begin(), itEnd = actions.end(); it != itEnd; ++it)
    {
        QAction *action = *it;
        BaseWidget *baseWidget = plugins_.at(action->data().toInt());
        action->setText(baseWidget->text());
        action->setStatusTip(baseWidget->statusTip());
    }

    // retranslate tab widget label
    for (int i = 0, size = ui->tabWidget->count(); i < size; ++i)
    {
        BaseWidget *baseWidget = static_cast<BaseWidget*>(ui->tabWidget->widget(i));
        ui->tabWidget->setTabText(i, baseWidget->text());
    }
}

void MainWindow::addConnection()
{
    ConnectDialog dialog(host_, port_);
    if (dialog.exec() == QDialog::Accepted)
    {
        host_ = dialog.host();
        port_ = dialog.port();

        static int position;
        clientsocket::ClientTcpSocket *clientSocket = new clientsocket::ClientTcpSocket(this);
        clientSocket->setNumber(++position);

        // connections
        //connect(&tcpSocket_, SIGNAL(disconnected()), this, SLOT(disconnect()));
        //connect(&tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
          //      this, SLOT(error(QAbstractSocket::SocketError)));
        //connect(&tcpSocket_, SIGNAL(readyRead()), this, SLOT(readData()));
        connect(clientSocket, SIGNAL(connected()), this, SLOT(login()));
        connect(clientSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(clientSocket, SIGNAL(sendAutorization(bool, const QString&)), this, SLOT(checkAutorization(bool, const QString&)));

        // some chckes
        std::map<int, clientsocket::ClientTcpSocket*>::iterator it = clientSockets_.find(position);
        Q_ASSERT(it == clientSockets_.end());
        clientSockets_.insert(std::make_pair(position, clientSocket));

        if (clientSocket->state() != QAbstractSocket::UnconnectedState)
        {
            // wait while disconnect without timeout
            clientSocket->disconnectFromHost();
            clientSocket->waitForDisconnected(-1);
        }
        clientSocket->connectToHost(host_, port_);

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        ui->treeWidget->setCurrentItem(item);
        QString connectionName = host_ + QLatin1Char(':') + QString::number(port_);
        item->setText(0, connectionName);

        // set item position index
        item->setData(0, Qt::UserRole, position);
        item->setData(0, Qt::UserRole + 1, host_);
        item->setData(0, Qt::UserRole + 2, port_);
    }
}

void MainWindow::removeConnection()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item)
    {
        if (QMessageBox::warning(this, QString::null, tr("Delete connection?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            const int position = item->data(0, Qt::UserRole).toInt();
            std::map<int, clientsocket::ClientTcpSocket*>::iterator it = clientSockets_.find(position);
            Q_ASSERT(it != clientSockets_.end());
            clientsocket::ClientTcpSocket *socket = it->second;
            delete socket;
            clientSockets_.erase(it);
            delete item;
        }
    }
    else
    {
        QMessageBox::information(this, tr("Input error"), tr("You must select a connection"));
    }
}

void MainWindow::fullView()
{
    int index = ui->tabWidget->currentIndex();
    if (index != -1)
    {
        // create full mode view
        QWidget *widget = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        widget->setParent(0);
        widget->installEventFilter(this);
        //widget->setWindowFlags(widget->windowFlags() | Qt::Window | Qt::CustomizeWindowHint);// | Qt::FramelessWindowHint) & ~Qt::WindowMinMaxButtonsHint & ~Qt::WindowCloseButtonHint);
        widget->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);

        fullViewPair_ = std::make_pair(index, widget);

        hide();
        widget->showMaximized();
    }
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    // return normal view
    if (object == fullViewPair_.second)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape)
            {
                const QString& text = static_cast<BaseWidget*>(fullViewPair_.second)->text();
                ui->tabWidget->insertTab(fullViewPair_.first, fullViewPair_.second, text);
                ui->tabWidget->setCurrentIndex(fullViewPair_.first);
                show();

                return true;
            }
        }
    }

    return QMainWindow::eventFilter(object, event);
}

clientsocket::ClientTcpSocket* MainWindow::getCurrentConnection()
{
    QTreeWidgetItem *currentItem = ui->treeWidget->currentItem();

    if (!currentItem)
    {
        if (!ui->treeWidget->topLevelItemCount())
        {
            addConnection();
            currentItem = ui->treeWidget->currentItem();
        }
        else
        {
            currentItem = ui->treeWidget->topLevelItem(0);
            currentItem->setSelected(true);
        }
    }

    if (currentItem)
    {
        const int position = currentItem->data(0, Qt::UserRole).toInt();
        std::map<int, clientsocket::ClientTcpSocket*>::iterator it = clientSockets_.find(position);
        return it->second;
    }
    else
    {
        // wtf??????????????? :)
        return 0;
    }
}

void MainWindow::socketStateChanged(QAbstractSocket::SocketState state)
{
    QString message;
    QColor color;

    switch (state)
    {
    case QAbstractSocket::UnconnectedState:
        message = tr("The socket is not connected");
        color = QColor(QLatin1String("#FF0000"));
        break;

    case QAbstractSocket::HostLookupState:
        message = tr("The socket is performing a host name lookup");
        color = QColor(QLatin1String("#00FF00"));
        break;
    case QAbstractSocket::ConnectingState:
        message = tr("The socket has started establishing a connection");
        color = QColor(QLatin1String("#00FF00"));
        break;

    case QAbstractSocket::ConnectedState:
        message = tr("A connection is established");
        color = QColor(QLatin1String("#0000FF"));
        break;

    case QAbstractSocket::ClosingState:
        message = tr("The socket is about to close (data may still be waiting to be written)");
        color = QColor(QLatin1String("#00FF00"));
        break;

    default:
        Q_ASSERT(false); // you souldn't be here
        break;
    }

    clientsocket::ClientTcpSocket *socket = qobject_cast<clientsocket::ClientTcpSocket*>(sender());
    if (socket)
    {
        int position = socket->number();
        if (position >= 0)
        {
            QTreeWidgetItemIterator it(ui->treeWidget);
            while (*it)
            {
                if ((*it)->data(0, Qt::UserRole).toInt() == position)
                {
                    (*it)->setText(1, message);
                    (*it)->setBackground(0, color);
                    (*it)->setBackground(1, color);
                    break;
                }
                ++it;
            }
        }
    }
}

void MainWindow::closeTab(int index)
{
    delete ui->tabWidget->widget(index);
}

void MainWindow::onTreeWidgetContexMenuRequested(const QPoint &point)
{
    QPoint globalPoint = ui->treeWidget->mapToGlobal(point);
    QTreeWidgetItem *item = ui->treeWidget->itemAt(point);

    if (item)
    {
        ui->treeWidget->setCurrentItem(item);
        QMenu contextMenu;
        contextMenu.addAction(connectAction_);
        contextMenu.addAction(disconnectAction_);

        contextMenu.exec(globalPoint);
    }
}

void MainWindow::connectToHost()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item)
    {
        int position = item->data(0, Qt::UserRole).toInt();
        QString host = item->data(0, Qt::UserRole + 1).toString();
        int port = item->data(0, Qt::UserRole + 2).toInt();

        std::map<int, clientsocket::ClientTcpSocket*>::iterator it = clientSockets_.find(position);
        if (it != clientSockets_.end())
        {
            clientsocket::ClientTcpSocket *socket = it->second;
            socket->connectToHost(host, port);
        }
    }
}

void MainWindow::disconnectFromHost()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item)
    {
        int position = item->data(0, Qt::UserRole).toInt();

        std::map<int, clientsocket::ClientTcpSocket*>::iterator it = clientSockets_.find(position);
        if (it != clientSockets_.end())
        {
            clientsocket::ClientTcpSocket *socket = it->second;
            socket->disconnectFromHost();
        }
    }
}

void MainWindow::login()
{
    QObject *object = sender();
    clientsocket::ClientTcpSocket *socket = qobject_cast<clientsocket::ClientTcpSocket*>(object);
    if (socket)
    {
        AurorizationDialog dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            socket->login(dlg.userName(), dlg.password());
        }
        else
        {
            socket->disconnectFromHost();
        }
    }
}

void MainWindow::checkAutorization(bool success, const QString &error)
{
    if (!success)
    {
        QMessageBox::information(this, tr("error"), error);
    }
}

void MainWindow::onTreeWidgetCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    // relese bold previous item
    if (previous)
    {
        QFont font = previous->font(0);
        font.setBold(false);
        previous->setFont(0, font);
        previous->setFont(1, font);
    }

    // set bold to new item
    if (current)
    {
        QFont font = current->font(0);
        font.setBold(true);
        current->setFont(0, font);
        current->setFont(1, font);
    }
}

void MainWindow::onTabWidgetCurrentChanged(int index)
{
    // set current item on tree widget
    BaseWidget *widget = qobject_cast<BaseWidget*>(ui->tabWidget->widget(index));
    if (widget)
    {
        clientsocket::ClientTcpSocket& socket = static_cast<clientsocket::ClientTcpSocket&>(widget->getClientSocket()); // bad!!!
        int position = socket.number();
        if (position >= 0)
        {
            QTreeWidgetItemIterator it(ui->treeWidget);
            while (*it)
            {
                if ((*it)->data(0, Qt::UserRole).toInt() == position)
                {
                    ui->treeWidget->setCurrentItem(*it);
                    break;
                }
                ++it;
            }
        }
    }
}
