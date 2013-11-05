#include <QCoreApplication>
#include <QtCore/QDebug>

#include <QtSerialPort/QSerialPortInfo>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }

    return a.exec();
}
