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
    CCustomerMonitor(CDBConnection *pDbConn, CSerialConnection *pSerialConn);

public slots:
    void process();

signals:
    void finished();
    void Error(QString err);

private:
    CSerialConnection *m_pSerialConn;
    CDBConnection *m_pDbConn;

    TCurrCostomers m_currCustomers;

    bool m_bTerminate;
};

#endif // CUSTOMERMONITOR_H
