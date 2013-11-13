#include "customermonitor.h"
#include "serialdefines.h"
#include "customerlist_shared.h"

#include <time.h>
#include <stdio.h>

#include <QByteArray>

#define SLEEP_INTERVAL_S    2

CCustomerMonitor::CCustomerMonitor(CDBConnection *pDbConn, CSerialConnection *pSerialConn)
    : m_pDbConn(pDbConn)
    , m_pSerialConn(pSerialConn)
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

                for(int i = 0; i < newList.size(); i++){
                    it = tmpCustomers.find(newList[i].cart_id);
                    if(it == tmpCustomers.end()){
                        // a new customer
                        cdata[CART_ID_BYTE_NO] = newList[i].cart_id;
                        data.append((char*)cdata, 2);
                        emit Error("New customer");
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
                    emit Error("Customer is gone");
                }

                if(m_pSerialConn->IsConnected())
                    m_pSerialConn->SendData(data);

                printf("size: %d 0x", data.size());
                for(int k(0); k < data.size(); k++){
                    printf("%02X", 0xFF & data.at(k));
                }
                printf("\n");
            }
            else
                emit Error("Failed to get customer list");
        }

        nanosleep(&tim, NULL);
    }

    emit finished();
}