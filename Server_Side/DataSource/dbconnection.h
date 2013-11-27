#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMutex>
#include <QStringList>

#include <vector>

#include "config.h"

typedef struct{
    int customer_id;
    char cart_id;
} TCustomerInfo;

typedef struct{
    int customer_id;
    int x;
    int y;
} TCustomerPos;

typedef std::vector<TCustomerInfo> TCustomerList;
typedef std::vector<TCustomerPos> TCustomerPosList;

typedef struct{
    int x;
    int y;
} TTile;

const TTile INVALID_TILE = {-1, -1};

class CDBConnection : public QObject
{
    Q_OBJECT

public:
    CDBConnection(TConnConfig *config, QObject *parent = 0);

    // returns true if successful
    bool Connect();

    bool IsConnected();

    void Disconnect();

    // returns true if successful
    bool GetCurrCustomers(TCustomerList &customers);

    // returns true if successful
    bool SetCurrCstmrPos(TCustomerPosList &pos);

    // returns INVALID_TILE if such tile not found
    TTile GetTile(char nodeIdA, char nodeIdB, char nodeIdC);

    int GetListModel(QSqlQueryModel &model);

public slots:
    void DeregCustomers(QStringList idsList);

signals:
    void Error(QString err);

private:
    TConnConfig *m_pConfig;
    QSqlDatabase m_db;

    QMutex mutex;
};

#endif // DBCONNECTION_H
