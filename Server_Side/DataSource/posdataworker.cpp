#include "posdataworker.h"
#include "customerlist_shared.h"

#include <map>
#include <vector>
#include <algorithm>
#include <stdio.h>

typedef struct{
    char cart_id;
    char node_id;
    int delay;
} __attribute__ ((packed)) TPackedData;

typedef struct{
    char node_id;
    int delay;
} __attribute__ ((packed)) TSingleCartData;

bool compare(TSingleCartData first, TSingleCartData second)
{
  if(first.delay < second.delay)
      return true;
  else
      return false;
}

CPosDataWorker::CPosDataWorker(CDBConnection *dbconn)
    : m_pDbConn(dbconn)
{
}

void CPosDataWorker::process()
{
    TCustomerPosList list;
    TCustomerPos pos;
    std::map<char, std::vector<TSingleCartData> > data; // map cart id and all node values
    std::map<char, std::vector<TSingleCartData> >::iterator it;

    TTile tile;

    printf("start pos process\n");

    // read the frame data and sort it to different nodes
    char *end_addr;
    TPackedData *dataPtr;

    dataPtr = (TPackedData *)frameData.data();
    end_addr = frameData.data() + frameData.size();

    while(dataPtr < (TPackedData *)end_addr){
        data[dataPtr->cart_id].push_back({dataPtr->node_id, dataPtr->delay});
        dataPtr++;
    }

    // get the position of each node with data
    for(it = data.begin(); it != data.end(); it++){
        std::sort(it->second.begin(), it->second.end(), compare);

        if(it->second.size() < 3)
            // need at least 3 time values
            continue;

        for(int i = 0; i < (int)(it->second.size() - 2); i++){

            // take lowest time and try to find tile they belong to
            tile = m_pDbConn->GetTile(it->second[i].node_id, it->second[i+1].node_id, it->second[i+2].node_id);

            if((tile.x != INVALID_TILE.x) && (tile.y != INVALID_TILE.y)){
                pos.customer_id = _currCustomers[it->first];
                pos.x = tile.x;
                pos.y = tile.y;

                list.push_back(pos);
                break;
            }
        }
    }
    m_pDbConn->SetCurrCstmrPos(list);
}
