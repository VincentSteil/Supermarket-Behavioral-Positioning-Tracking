#ifndef CSERIALCONNECTION_H
#define CSERIALCONNECTION_H

#include "config.h"

#include <QSerialPort>
#include <QObject>
#include <QByteArray>
#include <QMutex>

class CSerialConnection : public QObject
{
    Q_OBJECT

public:
    CSerialConnection(TConnConfig *config, QObject *parent = 0);

    // returns true if successful
    bool Connect();

    bool IsConnected();

    void Disconnect();

    void ReadNewFrameData(QByteArray &target);

public slots:
    void SendData(QByteArray &data);

signals:
    void Error(QString err);
    void FrameDataReady();

private slots:
    void ReadNewData();

private:

    enum EByteType{
        eFrameEnd,
        eCartPos,
        eSkip
    };

    EByteType IdReadByte(unsigned char byte);

    TConnConfig *m_connCfg;

    QSerialPort m_serialPort;

    QByteArray m_prevFrameData;
    QByteArray m_nxtFrameData;
    QMutex mutex;
};

#endif // CSERIALCONNECTION_H
