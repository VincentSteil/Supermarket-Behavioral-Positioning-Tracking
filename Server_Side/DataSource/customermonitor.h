#ifndef CUSTOMERMONITOR_H
#define CUSTOMERMONITOR_H

#include <QObject>

#include <map>

#include "serialconnection.h"
#include "dbconnection.h"
#include "customerlist_shared.h"

class CCustomerMonitor
        : public QObject
{
    Q_OBJECT

public:
    CCustomerMonitor(CDBConnection *pDbConn);

public slots:
    void process();

signals:
    void finished();
    void Error(QString err);
    void SendData(QByteArray data);

private:
    CDBConnection *m_pDbConn;

    TCurrCostomers m_currCustomers;

    bool m_bTerminate;
};

#endif // CUSTOMERMONITOR_H
