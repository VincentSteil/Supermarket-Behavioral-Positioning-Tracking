#include "customermonitor.h"
#include "serialdefines.h"
#include "customerlist_shared.h"

#include <time.h>
#include <stdio.h>

#include <QByteArray>

#define SLEEP_INTERVAL_S    2

CCustomerMonitor::CCustomerMonitor(CDBConnection *pDbConn)
    : m_pDbConn(pDbConn)
    , m_bTerminate(false)
{
}

void CCustomerMonitor::process()
{
    struct timespec tim;
    TCustomerList newList;
    TCurrCostomers::iterator it;
    TCurrCostomers tmpCustomers;
    QByteArray data;
    char cdata[2] = {0,0};

    tim.tv_sec = SLEEP_INTERVAL_S;  // seconds
    tim.tv_nsec = 0; // 0 ms

    while(true){
        if(m_bTerminate)
            break;

        if(m_pDbConn->IsConnected()){
            if(m_pDbConn->GetCurrCustomers(newList)){
                // got the list

                data.clear();
                tmpCustomers = m_currCustomers;
                m_currCustomers.clear();
                cdata[TYPE_BYTE_NO] = TYPE_START_CART_TRACKING;

                for(int i = 0; i < (int)newList.size(); i++){
                    it = tmpCustomers.find(newList[i].cart_id);
                    if(it == tmpCustomers.end()){
                        // a new customer
                        cdata[CART_ID_BYTE_NO] = newList[i].cart_id;
                        data.append((char*)cdata, 2);
                        emit Error("INFO: New customer");
                    }
                    else{
                        // remove it from tmpList
                        tmpCustomers.erase(it);
                    }
                    // keep/add customer
                    m_currCustomers[newList[i].cart_id] = newList[i].customer_id;
                }
                _currCustomers = m_currCustomers;


                // need to remove all legacy customers
                cdata[TYPE_BYTE_NO] = TYPE_STOP_CART_TRACKING;
                for(it = tmpCustomers.begin(); it != tmpCustomers.end(); it++){
                    cdata[CART_ID_BYTE_NO] = it->first;
                    data.append(cdata, 2);
                    emit Error("INFO: Customer is gone");
                }

                if(data.size() > 0)
                    emit SendData(data);
            }
            else
                emit Error("Failed to get customer list");
        }

        nanosleep(&tim, NULL);
    }

    emit finished();
}
