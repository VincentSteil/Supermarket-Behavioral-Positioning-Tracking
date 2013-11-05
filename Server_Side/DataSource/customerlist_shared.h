#ifndef CUSTOMERLIST_SHARED_H
#define CUSTOMERLIST_SHARED_H

#include <map>

// mapping between cart id (key) and customer id
typedef std::map<char, int> TCurrCostomers;

extern TCurrCostomers _currCustomers;

#endif // CUSTOMERLIST_SHARED_H
