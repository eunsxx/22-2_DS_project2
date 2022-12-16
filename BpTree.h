#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include <fstream>
#include <iostream>

class BpTree {
private:
    BpTreeNode* root;
    int order;		// m children
    ofstream* fout; // log.txt
public:
    BpTree(ofstream *fout, int order = 3) { // constructor
        root = NULL;
        this->order = order;
        this->fout = fout;
    }
    ~BpTree() {
        root->getDataMap()->clear();
        root->getIndexMap()->clear();
    }; // destructor
    /* essential */
    bool Insert(int key, set<string> set);
    bool excessDataNode(BpTreeNode* pDataNode);
    bool excessIndexNode(BpTreeNode* pIndexNode);
    void splitDataNode(BpTreeNode* pDataNode);
    void splitIndexNode(BpTreeNode* pIndexNode);
    BpTreeNode* getRoot() { return root; }
    BpTreeNode* searchDataNode(int n); // n is frequency.

    void printFrequentPatterns(set<string> pFrequentPattern);
    bool printFrequency(string item, int min_frequency);
    bool printConfidence(string item, double item_frequency, double min_confidence);
    bool printRange(string item, int min, int max);

};

#endif