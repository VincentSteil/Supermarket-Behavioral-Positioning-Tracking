#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

#include "ui_mainwidget.h"

#include "serialconnection.h"
#include "dbconnection.h"
#include "config.h"
#include "errorlog.h"
#include "deregwidget.h"

class CMainWidget
        : public QWidget
        , public Ui::MainWidget
{
    Q_OBJECT

public:
    CMainWidget(QWidget *parent = 0);
    ~CMainWidget();

public slots:
    void SettingsPanel();
    void HandleNewFrameData();
    void ShowErrLog();
    void ShowDereg();

    void ConnectSerial();
    void ConnectDB();

private:
    void LoadConfig();
    void SaveConfig();
    void StartCstmrMon();

    TConnConfig m_connCfg;
    CSerialConnection m_serialCon;
    CDBConnection m_dbCon;
    CErrorLog m_errLog;
    CDeregWidget *m_pdereg;
};

#endif // MAINWIDGET_H
