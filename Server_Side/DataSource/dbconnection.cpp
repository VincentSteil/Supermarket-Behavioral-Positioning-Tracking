#include "dbconnection.h"
#include "sqldefines.h"

#include <QSqlQuery>
#include <QVariant>
#include <QMutexLocker>

#include <QDebug>
#include <stdio.h>

CDBConnection::CDBConnection(TConnConfig *config, QObject *parent)
    : QObject(parent)
    , m_pConfig(config)
    , mutex(QMutex::Recursive)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

bool CDBConnection::Connect()
{
    m_db.setHostName(m_pConfig->dbHostName);
    m_db.setDatabaseName(m_pConfig->dbName);
    m_db.setUserName(m_pConfig->userName);
    m_db.setPassword(m_pConfig->pswd);

    return m_db.open();
}

bool CDBConnection::IsConnected()
{
    return m_db.isOpen();
}

void CDBConnection::Disconnect()
{
    m_db.close();
}

bool CDBConnection::GetCurrCustomers(TCustomerList &customers)
{
    TCustomerInfo entry;
    QMutexLocker locker(&mutex);

    if(!m_db.isOpen())
        return false;

    QSqlQuery query(GET_CUSTOMERS_LIST, m_db);
    customers.clear();

    while(query.next()){
        entry.customer_id = query.value(CUSTOMERS_LIST_CSTMR_ID).toInt();
        entry.cart_id = query.value(CUSTOMERS_LIST_CART_ID).toInt();

        customers.push_back(entry);
    }

    return true;
}

bool CDBConnection::SetCurrCstmrPos(TCustomerPosList &pos)
{
    bool ret = true;
    QMutexLocker locker(&mutex);
    if(!m_db.isOpen())
        return false;

    for(int i = 0; i < (int)pos.size(); i++){
        QSqlQuery query(INSERT_CART_POS, m_db);
        query.bindValue(CART_POS_BIND_CSTMR_ID, pos[i].customer_id);
        query.bindValue(CART_POS_BIND_X, pos[i].x);
        query.bindValue(CART_POS_BIND_Y, pos[i].y);
        ret = query.exec();
    }

    return ret;
}

TTile CDBConnection::GetTile(char nodeIdA, char nodeIdB, char nodeIdC)
{
    QMutexLocker locker(&mutex);
    if(!m_db.isOpen())
        return INVALID_TILE;

    TTile ret;
    QSqlQuery query(XY_FROM_3_TILES, m_db);
    query.bindValue(XY_FROM_TILES_POS_1TILE_ID, nodeIdA);
    query.bindValue(XY_FROM_TILES_POS_2TILE_ID, nodeIdB);
    query.bindValue(XY_FROM_TILES_POS_3TILE_ID, nodeIdC);

    if(query.exec()){
        if(query.size() == 1){
            query.next();
            ret.x = query.value(XY_FROM_TILES_X_COL).toInt();
            ret.y = query.value(XY_FROM_TILES_Y_COL).toInt();
        }
        else
            ret = INVALID_TILE;
    }
    else{
        ret = INVALID_TILE;
        emit Error("Failed to exute query to get tile");
    }

    return ret;
}
