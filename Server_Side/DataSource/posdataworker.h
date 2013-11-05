#ifndef POSDATAWORKER_H
#define POSDATAWORKER_H

#include <QObject>
#include <QByteArray>

#include "dbconnection.h"

class CPosDataWorker
        : public QObject
{
    Q_OBJECT

public:
    CPosDataWorker(CDBConnection *dbconn);

    // is public so that can belong to the class and set from other modules
    QByteArray frameData;

public slots:
    void process();

signals:
    void finished();

private:
    CDBConnection *m_pDbConn;
};

#endif // POSDATAWORKER_H
