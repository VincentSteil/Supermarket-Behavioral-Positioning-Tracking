#include "serialconnection.h"
#include "serialdefines.h"

bool posBlockNow = false;
int posBlockBytes = 0;

CSerialConnection::CSerialConnection(TConnConfig *config, QObject *parent)
    : QObject(parent)
    , m_connCfg(config)
    , m_serialPort(this)
    , mutex(QMutex::Recursive)
{
    connect(&m_serialPort, SIGNAL(readyRead()), this, SLOT(ReadNewData()));
}

bool CSerialConnection::Connect()
{
    bool ret;
    if(m_serialPort.isOpen())
        Disconnect();

    m_serialPort.setPortName(m_connCfg->portName);
    m_serialPort.setBaudRate(m_connCfg->baudRate);
    m_serialPort.setDataBits(m_connCfg->dataBits);
    m_serialPort.setParity(m_connCfg->parity);
    m_serialPort.setStopBits(m_connCfg->stopBits);

    ret = m_serialPort.open(QIODevice::ReadWrite);

    if(!ret)
        emit Error(m_serialPort.errorString());

    return ret;
}

bool CSerialConnection::IsConnected()
{
    return m_serialPort.isOpen();
}

void CSerialConnection::Disconnect()
{
    m_serialPort.close();
    posBlockNow = false;
    posBlockBytes = 0;
}

void CSerialConnection::SendData(QByteArray &data)
{
    int res;
    if(m_serialPort.isOpen()){
        res = m_serialPort.write(data);
        if(res == -1)
            emit Error("Error occurred on serial connection");
        else if(res != data.length())
            emit Error("Write/written mismatch");
    }
    else
        emit Error("Send data: serial connection is not available");
}

void CSerialConnection::ReadNewData()
{
    EByteType byteType;
    int numRead = 0;
    char buffer[50];

    while(true){
        numRead  = m_serialPort.read(buffer, 50);

        for(int i = 0; i < numRead; i++){
            byteType = IdReadByte((unsigned char) buffer[i]);
            if(byteType == eFrameEnd){
                {// new scope for mutex
                    QMutexLocker locker(&mutex);
                    m_prevFrameData.clear();
                    m_prevFrameData.swap(m_nxtFrameData);
                }
                emit FrameDataReady();
            }
            else if(byteType == eCartPos)
                m_nxtFrameData.push_back(buffer[i]);
        }

        if (numRead == 0)
            break;
    }
}

CSerialConnection::EByteType CSerialConnection::IdReadByte(unsigned char byte)
{
    EByteType ret;

    if(posBlockNow){
        posBlockBytes++;
        if(posBlockBytes == CART_POSITION_LEN){
            posBlockBytes = 0;
            posBlockNow = false;
        }
        ret = eCartPos;
    }
    else{
        switch(byte){
        case TYPE_END_OF_FRAME:
            ret = eFrameEnd;
            break;
        case TYPE_CART_POSITION:
            posBlockNow = true;
            posBlockBytes++;
            ret = eSkip; // we don't need to pass this byte since it only used in msg identification
            break;
        default:
            ret = eSkip;
            emit Error("Invalid byte found in stream");
        }
    }

    return ret;
}

void CSerialConnection::ReadNewFrameData(QByteArray &target)
{
    QMutexLocker locker(&mutex);
    target.clear();
    target.append(m_prevFrameData);
}
