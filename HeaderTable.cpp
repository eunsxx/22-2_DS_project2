#include "HeaderTable.h"
#include <iostream>

using namespace std;

HeaderTable::~HeaderTable() {
    indexTable.clear();
    dataTable.clear();
} // destructor

void HeaderTable::insertTable(string item, int frequency) {
    // create index table
    list<pair<int, string>>::iterator iter;
    FPNode* newNode = new FPNode;
    if(indexTable.empty()) { // if index table is empty
        pair<int, string> p = make_pair(frequency, item);
        indexTable.push_front(p); // insert frequency and item at front
        dataTable.insert({item, nullptr});
        return;
    } else { // if index table is not empty
        for (iter=indexTable.begin(); iter!= indexTable.end(); iter++) {
            if(iter->second == item) { // if you can find item in index table
                indexTable.erase(iter); // remove first
                dataTable.erase(item);
                indexTable.push_back(make_pair(frequency, item)); // insert back
                dataTable.insert({item, newNode});
                return;
            }
        }
        // if you can't find item in index table
        indexTable.push_back(make_pair(frequency, item)); // insert back
        return;
    }

    // create data table
}

int HeaderTable::find_frequency(string item) {
    int frequency = 0; // initialize
    list<pair<int, string>>::iterator iter;
    list<pair<int, string>> table = getindexTable();
    if (table.empty()) return frequency; // if index table is empty -> frequency is 0
    else { // if index table is not empty
        for (iter = table.begin(); iter != table.end(); iter++) {
            if (iter->second == item) { // if you can find item in index table
                return iter->first; // this item's frequency return
            }
        }
    }
    return frequency; // if you can't find item in index table, return 0
}