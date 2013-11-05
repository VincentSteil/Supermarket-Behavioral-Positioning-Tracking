#include "settingsdialog.h"

#include <QSerialPortInfo>

CSettingsDialog::CSettingsDialog(TConnConfig *config, QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    connect(okBtn, SIGNAL(clicked()), this, SLOT(OkSlot()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(CancelSlot()));

    hostLine->setText(config->dbHostName);
    dbnameLine->setText(config->dbName);
    usernameLine->setText(config->userName);
    pswdLine->setText(config->pswd);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portNamesBox->addItem(info.portName());
    }
    portNamesBox->setCurrentIndex(portNamesBox->findText(config->portName));

    baudBox->addItem("115200", QSerialPort::Baud115200);
    baudBox->addItem("57600", QSerialPort::Baud57600);
    baudBox->addItem("38400", QSerialPort::Baud38400);
    baudBox->addItem("19200", QSerialPort::Baud19200);
    baudBox->addItem("9600", QSerialPort::Baud9600);
    baudBox->addItem("4800", QSerialPort::Baud4800);
    baudBox->addItem("2400", QSerialPort::Baud2400);
    baudBox->addItem("1200", QSerialPort::Baud1200);
    baudBox->setCurrentIndex(baudBox->findData(config->baudRate));

    dataBox->addItem("5", QSerialPort::Data5);
    dataBox->addItem("6", QSerialPort::Data6);
    dataBox->addItem("7", QSerialPort::Data7);
    dataBox->addItem("8", QSerialPort::Data8);
    dataBox->setCurrentIndex(dataBox->findData(config->dataBits));

    parityBox->addItem("No Parity", QSerialPort::NoParity);
    parityBox->addItem("Even parity.", QSerialPort::EvenParity);
    parityBox->addItem("Odd parity", QSerialPort::OddParity);
    parityBox->addItem("Space parity.", QSerialPort::SpaceParity);
    parityBox->addItem("Mark parity", QSerialPort::MarkParity);
    parityBox->setCurrentIndex(parityBox->findData(config->parity));

    stopBox->addItem("1 stop bit", QSerialPort::OneStop);
    stopBox->addItem("1.5 stop bits", QSerialPort::OneAndHalfStop);
    stopBox->addItem("2 stop bits", QSerialPort::TwoStop);
    stopBox->setCurrentIndex(stopBox->findData(config->stopBits));

    m_pConfig = config;
}

void CSettingsDialog::OkSlot()
{
    m_pConfig->portName = portNamesBox->currentText();
    m_pConfig->baudRate = (QSerialPort::BaudRate) baudBox->itemData(baudBox->currentIndex()).toInt();
    m_pConfig->dataBits = (QSerialPort::DataBits) dataBox->itemData(dataBox->currentIndex()).toInt();
    m_pConfig->parity = (QSerialPort::Parity) parityBox->itemData(parityBox->currentIndex()).toInt();
    m_pConfig->stopBits = (QSerialPort::StopBits) stopBox->itemData(stopBox->currentIndex()).toInt();

    m_pConfig->dbHostName = hostLine->text();
    m_pConfig->dbName = dbnameLine->text();
    m_pConfig->userName = usernameLine->text();
    m_pConfig->pswd = pswdLine->text();

    accept();
}

void CSettingsDialog::CancelSlot()
{
    reject();
}
