
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QVariant>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "commonfunctions.h"
#include "picturesingleton.h"

PictureSingleton* PictureSingleton::instance_ = 0;

void parseCommandLine(int &port)
{
    QStringList argList = QCoreApplication::instance()->arguments();
    for (int i = 0; i < argList.size(); ++i)
    {
        if (argList[i] == "-m")
        {
#ifdef Q_OS_WIN
            const QString cInCircle("(C)");
#else
            const QChar cInCircle(0xA9);
#endif
            QString copyright = QLatin1String("Copyright ") + cInCircle + " Michael Mihalevich. All rights not reserved";
            qDebug() << copyright;
        }
        else if (argList[i] == "-p")
        {
            if (argList.value(i+1) != QString::null)
            {
                bool ok;
                int argPort = argList.value(i+1).toInt(&ok);
                if (ok)
                {
                    port = argPort;
                    ++i;
                }
            }
        }
    }
}

void checkAutorunSettings(bool automatic)
{
    // only on release mode
//#ifndef DEBUG

#if defined Q_OS_WIN
        QString applicationName(QCoreApplication::instance()->applicationName());
        QSettings autorun(QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
        QVariant application = autorun.value(applicationName);
        if (!application.isValid())
        {
            if (automatic)
            {
                // set app to autorun
                QString path = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
                path.prepend(QLatin1Char('\"'));
                path.append(QLatin1Char('\"'));
                autorun.setValue(applicationName, path);
            }
        }
        else
        {
            if (!automatic)
            {
                autorun.remove(applicationName);
            }
        }
#elif defined Q_OS_LINUX
    QString applicationPath(QCoreApplication::instance()->applicationFilePath());
    QString applicationName(QCoreApplication::instance()->applicationName());

    // set app to autorun on kde
    QFileInfo kdeDir(QDir::home().absolutePath() + QDir::separator() + ".kde");
    if (kdeDir.isDir() && kdeDir.exists())
    {
        QString autorunLink = kdeDir.absoluteFilePath() + QDir::separator() + "Autostart" + QDir::separator() + applicationName;
        QFileInfo link(autorunLink);

        if (automatic)
        {
            // set
            if (!link.exists())
            {
                QFile applicationFile(applicationPath);
                bool save = applicationFile.link(autorunLink);
                Q_ASSERT(save);
            }
        }
        else
        {
            // remove
            if (link.exists())
            {
                QFile fileLink(autorunLink);
                Q_ASSERT(fileLink.remove());
            }
        }
    }
#endif

//#endif // DEBUG
}
