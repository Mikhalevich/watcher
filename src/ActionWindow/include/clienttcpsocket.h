#ifndef CLIENTTCPSOCKET_H
#define CLIENTTCPSOCKET_H

#include <QTcpSocket>
#include <QDateTime>

#include "global.h"

using namespace datatypes;
using namespace params;

class ClientTcpSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientTcpSocket(QObject *parent = 0);
    ClientTcpSocket(int number, QObject *parent = 0);

    void setNumber(int number)
    {
        socketNumber_ = number;
    }

    int number() const
    {
        return socketNumber_;
    }

    // public functions
    void getAllWindows();
    void getAllControls(const QString& windowName);
    void setControlText(qint16 controlIndex, qint8 controlType, const QString& newControlText);
    void getControlInfo(qint16 controlIndex, qint8 controlType);
    void runProcess(const QString& processLine);

    // take screenshot
    void getPicture();
    // get all pictures in database
    void getPictures(const QDateTime begin = QDateTime(), const QDateTime end = QDateTime());

signals:
    void sendData(const AbstractData& data);

private slots:
    void readData();

private:
    void init();

    //QStringList allWindows_;
    //QStringList allControls_;

    // determine operation
    // 1 - get all windows
    // 2 - get all controls of one window
    // 3 - set controls contents (based on control type variable)
    // 4 - get info of one control (based on control type variable)
    qint8 operation_;

    int socketNumber_;
};

#endif // CLIENTTCPSOCKET_H
