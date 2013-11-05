#ifndef CONFIG_H
#define CONFIG_H

#include <QSerialPort>

typedef struct{
    // DB details
    QString dbHostName;
    QString dbName;
    QString userName;
    QString pswd;
    // Serial cable details
    QString portName;
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
} TConnConfig;

#endif // CONFIG_H
