
// spike for excluding pythonxx_d.lib
#ifdef _DEBUG
    #undef _DEBUG
    #include "Python.h"
    #define _DEBUG
#else
    #include "Python.h"
#endif


#include <QSharedPointer>
#include <QFile>

#include "picturetimer.h"
#include "databasemanager.h"
#include "picturesingleton.h"
#include "database/query/mailquery.h"
#include "database/query/picturequery.h"
#include "database/query/picturepropertiesquery.h"

PictureTimer::PictureTimer()
{
    connect(&pictureTimer_, SIGNAL(timeout()), this, SLOT(savePicture()));
    connect(&mailTimer_, SIGNAL(timeout()), this, SLOT(sendMail()));

    // set python home
    // all time living
    static const QByteArray searchPath = QCoreApplication::applicationDirPath().toUtf8() + QByteArray("/Python27/");
    char *pySearchPath = const_cast<char*>(searchPath.data());
    Py_SetPythonHome(pySearchPath);

    /*
    // for mail testing
    mailTimer_.setSingleShot(true);
    mailProperties_.server = "smtp.gmail.com";
    mailProperties_.serverPort = 587;
    mailProperties_.user = "formemichael@gmail.com";
    mailProperties_.password = "mihalevich";
    mailProperties_.sendFrom = "formemichael@gmail.com";
    mailProperties_.sendTo = QStringList() << "michael.mikhalevich@gmail.com";

    mailTimer_.start(1000);*/

    // request mail properties
    database::databasequery::GetMailPropertiesQuery querry;
    querry.execute();
    mailProperties_ = querry.mailParams();
    if (mailProperties_.interval > 0)
    {
        mailTimer_.start(mailProperties_.interval * 1000 * 60);
    }

    // request picture interval
    database::databasequery::GetPicturePropertiesQuery picQuerry;
    picQuerry.execute();
    int pictureInterval = picQuerry.interval();
    if (pictureInterval > 0)
    {
        pictureTimer_.start(pictureInterval * 1000);
    }


}

void PictureTimer::pictureTimer(int secInterval)
{
    if (secInterval > 0)
    {
        if (pictureTimer_.isActive())
        {
            pictureTimer_.stop();
        }

        pictureTimer_.start(secInterval * 1000);
    }
    else
    {
        pictureTimer_.stop();
    }

    // save picture interval to database
    database::databasetypes::sendDatabaseEvent(QSharedPointer<database::databasetypes::IBaseData>(new database::databasetypes::PicturePropertiesData(secInterval)),
                                         database::DatabaseManager::instance());
}

void PictureTimer::mailProperties(const QString &server, const int port,
                                  const QString &user, const QString &password,
                                  const QString &sendFrom, const QStringList &sendTo,
                                  int interval)
{
    database::databasetypes::MailParams oldParams(mailProperties_);

    // set parameters
    mailProperties_.server = server;
    mailProperties_.serverPort = port;
    mailProperties_.user = user;
    mailProperties_.password = password;
    mailProperties_.sendFrom = sendFrom;
    mailProperties_.sendTo = sendTo;
    mailProperties_.interval = interval;

    if (interval > 0)
    {
        // if change server or port or user or password or sendFrom or sendTo than we must send a test mail message
        if (mailProperties_ != oldParams)
        {
            sendTestMail();
        }

        if (mailTimer_.isActive())
        {
            mailTimer_.stop();
        }

        // start timer
        mailTimer_.start(interval * 1000 * 60);
    }
    else
    {
        // stop timer
        mailTimer_.stop();
    }

    // save params to database
    database::databasequery::StoreMailPropertiesQuery query(mailProperties_);
    query.execute();
}

void PictureTimer::savePicture()
{
    // take picture and save in picture method
    QMetaObject::invokeMethod(PictureSingleton::instance(), "takePicture", Qt::QueuedConnection, Q_ARG(QObject*, this));
}

void PictureTimer::picture(QImage image)
{
    // save taken picture to database
    QDateTime date(QDateTime::currentDateTime());

    database::databasequery::StorePictureQuery query(image, date);
    query.execute();
}

void PictureTimer::sendMail()
{
    database::databasequery::GetNewPictureQuery query;
    query.execute();

    const database::databasequery::GetNewPictureQuery::Conteiner& pictures = query.pictures();

    PyObject *pName, *pModule, *pFunc;
    PyObject *pReturnValue;
    PyObject *pPictures;

    // initialize interpretter
    Py_Initialize();

    // import module
    pName = PyUnicode_FromString("mail");
    pModule = PyImport_Import(pName);

    Py_DECREF(pName);

    if (pModule != NULL)
    {
        pFunc = PyObject_GetAttrString(pModule, "send_mail");
        if (pFunc && PyCallable_Check(pFunc))
        {
            // tuple when each object is tuple with two elements - date and picture
            // ((date1, picture1), (date2, picture2), ...)
            pPictures = PyTuple_New(pictures.size());
            for (int i = 0; i < pictures.size(); ++i)
            {
                const QString& date = pictures.at(i).first;
                const QByteArray& picture = pictures.at(i).second;

                PyObject *pPicture = PyByteArray_FromStringAndSize(picture.data(), picture.size());
                PyObject *pDate = PyString_FromString(date.toStdString().c_str());
                PyObject *pInnerTuple = PyTuple_New(2);
                PyTuple_SetItem(pInnerTuple, 0, pDate);
                PyTuple_SetItem(pInnerTuple, 1, pPicture);

                PyTuple_SetItem(pPictures, i, pInnerTuple);
                //Py_DECREF(pInnerTuple);
            }

            PyObject *pServer = PyUnicode_FromString(mailProperties_.server.toUtf8().data());
            PyObject *pPort = PyInt_FromLong(mailProperties_.serverPort);
            PyObject *pUser = PyUnicode_FromString(mailProperties_.user.toUtf8().data());
            PyObject *pPassword = PyUnicode_FromString(mailProperties_.password.toUtf8().data());
            PyObject *pSendFrom = PyUnicode_FromString(mailProperties_.sendFrom.toUtf8().data());

            PyObject *pSendTo = PyTuple_New(mailProperties_.sendTo.size());
            for (int i = 0; i < mailProperties_.sendTo.size(); ++i)
            {
                PyObject *recepient = PyUnicode_FromString(mailProperties_.sendTo.at(i).toUtf8().data());
                PyTuple_SetItem(pSendTo, i, recepient);
                //Py_DECREF(recepient);
            }

            PyObject *pArgs = PyTuple_New(7);
            PyTuple_SetItem(pArgs, 0, pPictures);
            PyTuple_SetItem(pArgs, 1, pServer);
            PyTuple_SetItem(pArgs, 2, pPort);
            PyTuple_SetItem(pArgs, 3, pUser);
            PyTuple_SetItem(pArgs, 4, pPassword);
            PyTuple_SetItem(pArgs, 5, pSendFrom);
            PyTuple_SetItem(pArgs, 6, pSendTo);

            pReturnValue = PyObject_CallObject(pFunc, pArgs);

            Py_XDECREF(pReturnValue);

            // this objects steal setItem function
            /*Py_DECREF(pPictures);
            *Py_DECREF(pServer);
            *Py_DECREF(pPort);
            *Py_DECREF(pUser);
            *Py_DECREF(pPassword);
            *Py_DECREF(pSendFrom);
            *Py_DECREF(pSendTo);
            */
            Py_DECREF(pArgs);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    // close interpretter
    Py_Finalize();
}

void PictureTimer::sendTestMail()
{
    PyObject *pName, *pModule, *pFunc;
    PyObject *pReturnValue;

    // initialize interpretter
    Py_Initialize();

    // import module
    pName = PyUnicode_FromString("mail");
    pModule = PyImport_Import(pName);

    Py_DECREF(pName);

    if (pModule != NULL)
    {
        pFunc = PyObject_GetAttrString(pModule, "send_test_mail");
        if (pFunc && PyCallable_Check(pFunc))
        {
            PyObject *pServer = PyUnicode_FromString(mailProperties_.server.toUtf8().data());
            PyObject *pPort = PyInt_FromLong(mailProperties_.serverPort);
            PyObject *pUser = PyUnicode_FromString(mailProperties_.user.toUtf8().data());
            PyObject *pPassword = PyUnicode_FromString(mailProperties_.password.toUtf8().data());
            PyObject *pSendFrom = PyUnicode_FromString(mailProperties_.sendFrom.toUtf8().data());

            PyObject *pSendTo = PyTuple_New(mailProperties_.sendTo.size());
            for (int i = 0; i < mailProperties_.sendTo.size(); ++i)
            {
                PyObject *recepient = PyUnicode_FromString(mailProperties_.sendTo.at(i).toUtf8().data());
                PyTuple_SetItem(pSendTo, i, recepient);
                //Py_DECREF(recepient);
            }

            PyObject *pArgs = PyTuple_New(6);
            PyTuple_SetItem(pArgs, 0, pServer);
            PyTuple_SetItem(pArgs, 1, pPort);
            PyTuple_SetItem(pArgs, 2, pUser);
            PyTuple_SetItem(pArgs, 3, pPassword);
            PyTuple_SetItem(pArgs, 4, pSendFrom);
            PyTuple_SetItem(pArgs, 5, pSendTo);

            pReturnValue = PyObject_CallObject(pFunc, pArgs);

            Py_XDECREF(pReturnValue);
            Py_DECREF(pArgs);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }
    // close interpretter
    Py_Finalize();
}
