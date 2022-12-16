#include "FPNode.h"
#include <iostream>

using namespace std;
FPNode::FPNode() { // constructor
    parent = NULL;
    next = NULL;
    frequency = 0;
}

FPNode::~FPNode() { // destructor
    children.clear();
}

FPNode* FPNode::getChildrenNode(string item) {
    map<string, FPNode*>::iterator it = children.find(item); // iterator 'it' declare, find 'item'
    if (it == children.end()) return nullptr; // if not find, iterator return children.end()
    FPNode* findNode = it->second; // second is FPNode* type
    return findNode;
}