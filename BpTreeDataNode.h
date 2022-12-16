#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"
class BpTreeDataNode : public BpTreeNode {
private:
    map <int, FrequentPatternNode*> mapData; // information of BpTreeDataNode. first is frequency, second is patterns
    BpTreeNode* pNext;
    BpTreeNode* pPrev;
public:
    BpTreeDataNode() { // constructor
        pNext = NULL;
        pPrev = NULL;
    }

    void setNext(BpTreeNode* pN) { pNext = pN; }
    void setPrev(BpTreeNode* pN) { pPrev = pN; }
    BpTreeNode* getNext() { return pNext; }
    BpTreeNode* getPrev() { return pPrev; }

    void insertDataMap(int n, FrequentPatternNode* pN) { // n is frequency, pN is multimap
        mapData.insert(map<int, FrequentPatternNode*>::value_type(n, pN));
    }

    void deleteMap(int n) {
        mapData.erase(n);
    }
    map<int, FrequentPatternNode*>* getDataMap() { return &mapData; }
};

#endif