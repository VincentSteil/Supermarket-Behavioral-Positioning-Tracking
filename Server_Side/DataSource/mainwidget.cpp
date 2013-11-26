#include "mainwidget.h"
#include "settingsdialog.h"
#include "posdataworker.h"
#include "customermonitor.h"

#include <QSettings>
#include <QSerialPortInfo>
#include <QThread>
#include <QApplication>
#include <QProgressDialog>
#include <QMessageBox>

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
    , m_dbCon(&m_connCfg, this)
    , m_serialCon(&m_connCfg, this)
    , m_errLog(this)
{
    setupUi(this);

    m_pdereg = new CDeregWidget(&m_dbCon, this);

    LoadConfig();

    connect(&m_serialCon, SIGNAL(FrameDataReady()), this, SLOT(HandleNewFrameData()));
    connect(&m_serialCon, SIGNAL(Error(QString)), &m_errLog, SLOT(ShowError(QString)));
    connect(exitBtn, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(dbBtn, SIGNAL(clicked()), this, SLOT(ConnectDB()));
    connect(serialBtn, SIGNAL(clicked()), this, SLOT(ConnectSerial()));

    StartCstmrMon();
}

CMainWidget::~CMainWidget()
{
    SaveConfig();
}

void CMainWidget::ShowErrLog()
{
    m_errLog.show();
}

void CMainWidget::SettingsPanel()
{
    CSettingsDialog dialog(&m_connCfg, this);

    dialog.exec();
}

void CMainWidget::LoadConfig()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "StarSoft", "Customer Tracking");

    m_connCfg.dbHostName = settings.value("dbconn/hostName", "").toString();
    m_connCfg.dbName = settings.value("dbconn/dbName", "").toString();
    m_connCfg.userName = settings.value("dbconn/userName", "").toString();
    m_connCfg.pswd = settings.value("dbconn/pswd", "").toString();

    m_connCfg.portName = settings.value("serialconn/portName", "").toString();
    m_connCfg.baudRate = (QSerialPort::BaudRate) settings.value("serialconn/baudRate", QSerialPort::Baud115200).toInt();
    m_connCfg.dataBits = (QSerialPort::DataBits) settings.value("serialconn/dataBits", QSerialPort::Data8).toInt();
    m_connCfg.parity = (QSerialPort::Parity) settings.value("serialconn/parity", QSerialPort::NoParity).toInt();
    m_connCfg.stopBits = (QSerialPort::StopBits) settings.value("serialconn/stopBits", QSerialPort::OneStop).toInt();
}

void CMainWidget::SaveConfig()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "StarSoft", "Customer Tracking");

    settings.setValue("dbconn/hostName", m_connCfg.dbHostName);
    settings.setValue("dbconn/dbName", m_connCfg.dbName);
    settings.setValue("dbconn/userName", m_connCfg.userName);
    settings.setValue("dbconn/pswd", m_connCfg.pswd);

    settings.setValue("serialconn/portName", m_connCfg.portName);
    settings.setValue("serialconn/baudRate", m_connCfg.baudRate);
    settings.setValue("serialconn/dataBits", m_connCfg.dataBits);
    settings.setValue("serialconn/parity", m_connCfg.parity);
    settings.setValue("serialconn/stopBits", m_connCfg.stopBits);
}

void CMainWidget::HandleNewFrameData()
{
    QThread *workerThread = new QThread;
    CPosDataWorker *worker = new CPosDataWorker(&m_dbCon);
    m_serialCon.ReadNewFrameData(worker->frameData);
    worker->moveToThread(workerThread);

    connect(workerThread, SIGNAL(started()), worker, SLOT(process()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

    connect(worker, SIGNAL(finished()), workerThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));

    workerThread->start();
}

void CMainWidget::ConnectDB()
{
    QMessageBox mbox;
    QProgressDialog progress("Connecting...", "Close", 0, 0, this);

    bool res = m_dbCon.Connect();

    // hide progress bar
    progress.cancel();

    if(res){
        mbox.setText("Connection successful");
        m_pdereg->SetListView();
    }
    else{
        mbox.setText("Connection failed");
        mbox.setInformativeText("Please check your settings");
        mbox.setIcon(QMessageBox::Critical);
    }

    mbox.setStandardButtons(QMessageBox::Close);
    mbox.exec();
}

void CMainWidget::ConnectSerial()
{
    QMessageBox mbox;
    bool res = m_serialCon.Connect();
    mbox.setStandardButtons(QMessageBox::Close);
    if(res){
        mbox.setText("Connection successful");
    }
    else{
        mbox.setText("Connection failed");
        mbox.setInformativeText("Please check your settings");
        mbox.setIcon(QMessageBox::Critical);
    }

    mbox.exec();
}

void CMainWidget::StartCstmrMon()
{
    QThread *workerThread = new QThread;
    CCustomerMonitor *worker = new CCustomerMonitor(&m_dbCon);
    worker->moveToThread(workerThread);

    connect(workerThread, SIGNAL(started()), worker, SLOT(process()));
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

    connect(worker, SIGNAL(finished()), workerThread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(Error(QString)), &m_errLog, SLOT(ShowError(QString)));
    connect(worker, SIGNAL(SendData(QByteArray)), &m_serialCon, SLOT(SendData1(QByteArray)));

    // when we send data over cable something changed -> trigger view update
    connect(worker, SIGNAL(SendData(QByteArray)), m_pdereg, SLOT(SetListView()));

    workerThread->start();
}

void CMainWidget::ShowDereg()
{
    m_pdereg->show();
}
