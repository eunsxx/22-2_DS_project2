#include "FPNode.h"
#include <iostream>
#include <list>

class HeaderTable {
private:
    list<pair<int, string>> indexTable; // save frequency(int) and product(string)
    map<string, FPNode*> dataTable; // save product(string) and product pointer(FPNode*)

public:
    HeaderTable() {} // constructor
    ~HeaderTable(); // destructor

    void insertTable(string item, int frequency); // insert product name, frequency in header table
    list<pair<int, string>> getindexTable() { return indexTable; } // return index table
    map<string, FPNode*> getdataTable() { return dataTable; } // return data table
    FPNode* getNode(string item) { return dataTable.find(item)->second; } // second is FPNode* type
    void descendingIndexTable() { indexTable.sort(greater<pair<int, string>>());} // sort index table in descending
    void ascendingIndexTable() { indexTable.sort(); } // sort index table in ascending
    int find_frequency(string item); // find item's frequency
};