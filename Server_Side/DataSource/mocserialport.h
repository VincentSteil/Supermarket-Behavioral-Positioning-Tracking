#ifndef MOCSERIALPORT_H
#define MOCSERIALPORT_H

#include <QObject>
#include <QTimer>
#include <QByteArray>

#include <QSerialPort>

class MocSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit MocSerialPort(QObject *parent = 0);

    void setPortName(const QString &name)
    {name;}

    bool setBaudRate(qint32 baudRate, QSerialPort::Directions dir = QSerialPort::AllDirections)
    {baudRate; dir; return true;}

    bool setDataBits(QSerialPort::DataBits dataBits)
    {dataBits; return true;}

    bool setParity(QSerialPort::Parity parity)
    { parity; return true; }

    bool setStopBits(QSerialPort::StopBits stopBits)
    { stopBits; return true; }

    bool open(QSerialPort::OpenMode mode);

    QString errorString() const
    { return QString(""); }

    bool isOpen() const
    { return m_isConnected; }

    void close()
    { m_timer->stop(); m_isConnected = false; }

    qint64 write(const QByteArray &data)
    { return data.size(); }

    qint64 read(char *data, qint64 maxlen);

signals:
    void readyRead();

private slots:
    void PrepareData();

private:
    bool m_isConnected;

    QTimer *m_timer;

    QByteArray m_data;
};

#endif // MOCSERIALPORT_H
