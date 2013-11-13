#include "mocserialport.h"

#include <stdio.h>

MocSerialPort::MocSerialPort(QObject *parent) :
    QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(PrepareData()));
}

bool MocSerialPort::open(QSerialPort::OpenMode mode)
{
    mode;
    m_isConnected = true;
    m_timer->start(1000);
    return true;
}

void MocSerialPort::PrepareData()
{
    char cdata[] = {0xFF, 0x12, 0x05, 0x12,0x0,0x0,0x0,
                    0xFF, 0x12, 0x02, 0x12,0x0,0x0,0x0,
                    0xFF, 0x12, 0x03, 0x12,0x0,0x0,0x0,
                    0xAF};
    m_data.append(cdata, 7*3+1);

    emit readyRead();
}

qint64 MocSerialPort::read(char *data, qint64 maxlen)
{
    int size = maxlen < m_data.size() ? maxlen : m_data.size();
    memcpy(data, m_data.data(), size);
    m_data = m_data.mid(size);
    return size;
}
