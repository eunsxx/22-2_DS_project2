#include <iomanip>
#include "math.h"
#include "BpTree.h"
bool BpTree::Insert(int key, set<string> itemSet) {
    if (root == nullptr) { // if b+ tree empty
        BpTreeNode* newDataNode = new BpTreeDataNode; // make a new Data Node
        FrequentPatternNode* newPattern = new FrequentPatternNode; // make a new Pattern Node
        newPattern->setFrequency(key); // set frequency new pattern Node
        newPattern->InsertList(itemSet); // Insert Item List in new pattern Node
        newDataNode->insertDataMap(key, newPattern); // insert new Pattern Node in a New Data Node
        root = newDataNode; // root is new Data Node
        return true;
    } else { // if b+ tree exists
        BpTreeNode* findDataMap = searchDataNode(key); // find same value of key in data nodes
        if (findDataMap != nullptr) { // if find Data Map
            map<int, FrequentPatternNode*>::iterator findMapData = findDataMap->getDataMap()->begin();
            while (findMapData != findDataMap->getDataMap()->end()) {
                if (findMapData->first == key) {
                    findMapData->second->InsertList(itemSet); // insert item list in a frequent Pattern Node
                    return true;
                }
                findMapData++;
            }
        } else { // not find Data Map
            BpTreeNode *newDataNode = root; // new data node is root
            while (newDataNode->getIndexMap() != nullptr) { // if root is index node
                map<int, BpTreeNode *>::iterator findIndex = newDataNode->getIndexMap()->begin(); // findIndex = first index node
                if (key < findIndex->first) newDataNode = newDataNode->getMostLeftChild(); // if n < first index, go to most Left Child Node
                else { // if n > first index, compare next index
                    while (findIndex != newDataNode->getIndexMap()->end()) {
                        if (key < findIndex->first) {
                            newDataNode = (--findIndex)->second; // findIndex->second is BpTreeNode*
                            findIndex = newDataNode->getIndexMap()->end(); // end state
                        } else if (findIndex->first <= key) {
                            ++findIndex;
                            if (findIndex == newDataNode->getIndexMap()->end()) {
                                --findIndex;
                                newDataNode = findIndex->second;
                                findIndex = newDataNode->getIndexMap()->end(); // end state
                            }
                        }
                    }
                }

            }

            FrequentPatternNode *newPattern = new FrequentPatternNode;
            newPattern->setFrequency(key);
            newPattern->InsertList(itemSet);
            newDataNode->insertDataMap(key, newPattern);
			
            if (excessDataNode(newDataNode)) splitDataNode(newDataNode); // if siae > order, split
            return true;
        }
    }
    return false;
}

BpTreeNode* BpTree::searchDataNode(int n) { // n is key(frequency). // begin~end까지 돌면서 해당하는 key 있는지 확인하기
    BpTreeNode* pCur = root;
    bool indexState = false;
    while (pCur->getIndexMap() != nullptr) { // if index node exists
        indexState = true;
        map<int, BpTreeNode *>::iterator findIndex = pCur->getIndexMap()->begin(); // findIndex = first index node

        if (n < pCur->getIndexMap()->begin()->first) {
            pCur = pCur->getMostLeftChild(); // if n < first index, go to most Left Child Node
            continue;
        }
        else { // if n > first index, compare next index
            while (findIndex != pCur->getIndexMap()->end()) {
                if (n < findIndex->first) { //findIndex->first is key
                    pCur = (--findIndex)->second; // findIndex->second is BpTreeNode*
                    findIndex = pCur->getIndexMap()->end(); // end state
                } else if (findIndex->first <= n) {
                    findIndex++;
                    if (findIndex == pCur->getIndexMap()->end()) {
                        --findIndex;
                        pCur = findIndex->second;
                        findIndex = pCur->getIndexMap()->end(); // end state
                    }
                }
            }
            continue;
        }

        map<int, FrequentPatternNode*>::iterator iter = pCur->getDataMap()->begin(); // maximum map is order
        cout << pCur->getDataMap()->begin()->first << endl;
        while (iter != pCur->getDataMap()->end()) {
            if (iter->first == n) {
                return pCur; // already exist
            } else iter++;
        }
        return nullptr;
    }

    if (indexState) {
        map<int, FrequentPatternNode*>::iterator iter = pCur->getDataMap()->begin(); // maximum map is order
        while (iter != pCur->getDataMap()->end()) {
            if (iter->first == n) {
                return pCur; // already exist
            } iter++;
        }
        return nullptr;
    } else {
        map<int, FrequentPatternNode *>::iterator iter = pCur->getDataMap()->begin(); // maximum map is order
        while (pCur != nullptr) {
            while (iter != pCur->getDataMap()->end()) {
                if (iter->first == n) {
                    return pCur; // already exist
                } else iter++;
            }
            pCur = pCur->getNext();
        }
        return pCur;
    }
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    bool nextNodeState = false; // if split node's next node already exist
    if (pDataNode->getNext() != nullptr) nextNodeState = true;
    int place = ceil((order-1)/2.0)+1; // it is split place

    map<int, FrequentPatternNode*>::iterator splitDataNode = pDataNode->getDataMap()->begin();

    int count = 1;
    while (count < place) {
        splitDataNode++;
        count++;
    }

    int splitKey = splitDataNode->first;
    map<int, FrequentPatternNode*>::iterator eraseNode = splitDataNode; // save data node which place split num

    // save data map to a new data node. it range is split num ~ end
    BpTreeNode* newDataNode_afterPlace = new BpTreeDataNode; // second data node
    while (splitDataNode != pDataNode->getDataMap()->end()) {
        FrequentPatternNode* pattern = splitDataNode->second;
        int key = pattern->getFrequency();
        newDataNode_afterPlace->insertDataMap(key, pattern);
        splitDataNode++;
    }

    // The nodes after the split num in the original data node are deleted.
    pDataNode->getDataMap()->erase(eraseNode, pDataNode->getDataMap()->end());


    // Let the index node point to the newly created data node.
    if (pDataNode->getParent() == nullptr) { // if index node not exist
        BpTreeNode *newIndexNode = new BpTreeIndexNode; // new index node 생성
        newIndexNode->insertIndexMap(splitKey, newDataNode_afterPlace); // insert Index map in a new index node
        root = newIndexNode; // root is new index node
        newIndexNode->setMostLeftChild(pDataNode); // index node's Most Left Child is origin node
        newDataNode_afterPlace->setNext(pDataNode->getNext());
        pDataNode->setParent(newIndexNode); // origin node's parent is new index node
        pDataNode->setNext(newDataNode_afterPlace); // origin node's next is new data node
        newDataNode_afterPlace->setPrev(pDataNode); // new data node's prev is origin node
        newDataNode_afterPlace->setParent(newIndexNode); // new data node's parent is new index node
    } else { // if index node already exist
        BpTreeNode* newIndexNode = pDataNode->getParent(); // index node = Data node's parents
        newIndexNode->insertIndexMap(splitKey, newDataNode_afterPlace); // add in index node
        newDataNode_afterPlace->setNext(pDataNode->getNext());
        pDataNode->setNext(newDataNode_afterPlace); // origin node's next is new data node
        newDataNode_afterPlace->setPrev(pDataNode); // new data node's prev is origin node
        newDataNode_afterPlace->setParent(newIndexNode); // new data node's parent is origin node's parent
        if (excessIndexNode(newIndexNode)) { // if origin node's parint.size == order, split
            splitIndexNode(newIndexNode);
        }
    }


}


void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    int place = ceil((order-1)/2.0)+1;
    map<int, BpTreeNode*>::iterator splitIndex = pIndexNode->getIndexMap()->begin();
    int count = 1;
    while (count < place) {
        splitIndex++;
        count++;
    }

    // Currently, splitIndexNode refers to split place.
    int splitKey = splitIndex->first;
    map<int, BpTreeNode*>::iterator eraserNode = splitIndex; // save split node

    // Save the data map to a new index node, range is split place ~ end. except split place.
    BpTreeNode* newIndexNode_after = new BpTreeIndexNode; // new index node
    newIndexNode_after->setMostLeftChild(splitIndex->second); // current splitIndex is split place.
    // new index node's most left child is split index's bpTreeNode. It may be a data node or an index node.
    splitIndex->second->setParent(newIndexNode_after);
    splitIndex++;
    while (splitIndex != pIndexNode->getIndexMap()->end()) { // save in newIndexNode_after
        int index = splitIndex->first;
        BpTreeNode* childNode = splitIndex->second;
        newIndexNode_after->insertIndexMap(index, childNode);
        childNode->setParent(newIndexNode_after);
        splitIndex++;
    }

    if (pIndexNode->getParent() == nullptr) { // if parent not exists
        BpTreeNode* newIndexNode = new BpTreeIndexNode; // make a new index node to save a split index
        newIndexNode->insertIndexMap(splitKey, newIndexNode_after); // insert a split index node in a new index node
        newIndexNode->setMostLeftChild(pIndexNode); // new split index node's most left child is origin node
        pIndexNode->setParent(newIndexNode); // origin node's parent is new split index node
        newIndexNode_after->setParent(newIndexNode); // new index node's parent is new split index node
        root = newIndexNode; // root is split index node
    } else { // if parent node exists
        BpTreeNode* parent = pIndexNode->getParent();
        parent->insertIndexMap(splitKey, newIndexNode_after); //insert a split index node in a origin's parent node
        newIndexNode_after->setParent(parent); // new index node's parent is origin's parent node
        if (excessIndexNode(parent)) { // After insert, origin's parent node.size == order, split again.
            splitIndexNode(parent);
        }
    }

    // The index node after split place is deleted in origin node
    pIndexNode->getIndexMap()->erase(eraserNode, pIndexNode->getIndexMap()->end());
}
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements
    else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements
    else return false;
}


bool BpTree::printConfidence(string item, double item_frequency, double min_confidence) {
    bool print_state = false;
    if (root == nullptr) return false; // If b+ tree not exists
    else {
        int min_frequency = ceil(min_confidence * item_frequency);
        BpTreeNode* findNode = searchDataNode(min_frequency);
        if (findNode == nullptr) return false;
        else {
            while (findNode != nullptr) {
                map<int, FrequentPatternNode*>::iterator mapData = findNode->getDataMap()->begin();
                while (mapData != findNode->getDataMap()->end()) {
                    if (mapData->first >= min_frequency) {
                        multimap<int, set<string>> patternList = mapData->second->getList();
                        multimap<int, set<string>>::iterator findPattern = patternList.begin();
                        while (findPattern != patternList.end()) {
                            set<string>::iterator item_list = findPattern->second.begin();
                            while (item_list != findPattern->second.end()) {
                                if (item == *item_list) {
                                    print_state = true;
                                }
                                item_list++;
                            }
                            findPattern++;
                        }
                    }
                    mapData++;
                }
                findNode = findNode->getNext();
            } // print_state setting
            if (print_state == false) return false;
            findNode = searchDataNode(min_frequency); // initialize
            *fout << "FrequentPattern\tFrequency\tConfidence" << endl;
            while (findNode != nullptr) {
                map<int, FrequentPatternNode*>::iterator mapData = findNode->getDataMap()->begin();
                while (mapData != findNode->getDataMap()->end()) {
                    if (mapData->first >= min_frequency) {
                        multimap<int, set<string>> patternList = mapData->second->getList();
                        multimap<int, set<string>>::iterator findPattern = patternList.begin();
                        while (findPattern != patternList.end()) {
                            set<string> frequent_Pattern = findPattern->second;
                            set<string>::iterator item_list = findPattern->second.begin();
                            while (item_list != findPattern->second.end()) {
                                if (item == *item_list) {
                                    printFrequentPatterns(frequent_Pattern); // print one item_set
                                    *fout << mapData->first << " ";
									fout->precision(1); // print .xx
                                    double confidence = mapData->first / item_frequency;
                                    *fout << confidence << endl;
                                }
                                item_list++;
                            }
                            findPattern++;
                        }
                    }
                    mapData++;
                }
                findNode = findNode->getNext();
            }
            return true;
        }
    }
}
bool BpTree::printFrequency(string item, int min_frequency) { //print winratio in ascending order
    bool print_state = false;
    if (root == nullptr) return false;
    BpTreeNode* findNode = searchDataNode(min_frequency);
    if (findNode != nullptr) {
        // print state check
        while (findNode != nullptr) {
            map<int, FrequentPatternNode *>::iterator findMapData = findNode->getDataMap()->begin();
            while (findMapData != findNode->getDataMap()->end()) {
                if (findMapData->first >= min_frequency) {
                    FrequentPatternNode *frequentPattern = findMapData->second;
                    multimap<int, set<string>> patternList = frequentPattern->getList();
                    multimap<int, set<string>>::iterator findItemList = patternList.begin();
                    while (findItemList != patternList.end()) {
                        set<string> frequent_Pattern = findItemList->second;
                        set<string>::iterator item_list = findItemList->second.begin();
                        while (item_list != findItemList->second.end()) {
                            if (item == *item_list) {
                                print_state = true;
                            }
                            item_list++;
                        }
                        findItemList++;
                    }
                }
                findMapData++;
            }
            findNode = findNode->getNext();
        }
        if (!print_state) return false;
        else {
            findNode = searchDataNode(min_frequency); // initialize
            *fout << "FrequentPattern\tFrequency" << endl;
            while (findNode != nullptr) {
                map<int, FrequentPatternNode *>::iterator findMapData = findNode->getDataMap()->begin();
                while (findMapData != findNode->getDataMap()->end()) {
                    if (findMapData->first >= min_frequency) {
                        FrequentPatternNode *frequentPattern = findMapData->second;
                        multimap<int, set<string>> patternList = frequentPattern->getList();
                        multimap<int, set<string>>::iterator findItemList = patternList.begin();
                        while (findItemList != patternList.end()) {
                            set<string> frequent_Pattern = findItemList->second;
                            set<string>::iterator item_list = findItemList->second.begin();
                            while (item_list != findItemList->second.end()) {
                                if (item == *item_list) {
                                    printFrequentPatterns(frequent_Pattern);
                                    *fout << findMapData->first << endl;
                                }
                                item_list++;
                            }
                            findItemList++;
                        }
                    }
                    findMapData++;
                }
                findNode = findNode->getNext();
            }
            return true;
        }
    } else return false; // not find frequency value in data nodes
}
bool BpTree::printRange(string item, int min, int max) {
    bool print_state = false;
    if (root == nullptr) return false;
    BpTreeNode* findNode = searchDataNode(min);
    if (findNode != nullptr) {
        // print state check
        while (findNode->getNext() != nullptr) {
            map<int, FrequentPatternNode *>::iterator findMapData = findNode->getDataMap()->begin();
            while (findMapData != findNode->getDataMap()->end()) {
                if (min <= findMapData->first && findMapData->first <= max) {
                    FrequentPatternNode *frequentPattern = findMapData->second;
                    multimap<int, set<string>> patternList = frequentPattern->getList();
                    multimap<int, set<string>>::iterator findItemList = patternList.begin();
                    while (findItemList != patternList.end()) {
                        set<string> frequent_Pattern = findItemList->second;
                        set<string>::iterator item_list = findItemList->second.begin();
                        while (item_list != findItemList->second.end()) {
                            if (item == *item_list) {
                                print_state = true;
                            }
                            item_list++;
                        }
                        findItemList++;
                    }
                }
                findMapData++;
            }
            findNode = findNode->getNext();
        } // print_state setting
        if (!print_state) return false;
        else {
            findNode = searchDataNode(min); // initialize
            *fout << "FrequentPattern\tFrequency" << endl;
            while (findNode != nullptr) {
                map<int, FrequentPatternNode *>::iterator findMapData = findNode->getDataMap()->begin();
                while (findMapData != findNode->getDataMap()->end()) {
                    if (min <= findMapData->first && findMapData->first <= max) {
                        FrequentPatternNode *frequentPattern = findMapData->second;
                        multimap<int, set<string>> patternList = frequentPattern->getList();
                        multimap<int, set<string>>::iterator findItemList = patternList.begin();
                        while (findItemList != patternList.end()) {
                            set<string> frequent_Pattern = findItemList->second;
                            set<string>::iterator item_list = findItemList->second.begin();
                            while (item_list != findItemList->second.end()) {
                                if (item == *item_list) {
                                    printFrequentPatterns(frequent_Pattern);
                                    *fout << findMapData->first << endl;
                                }
                                item_list++;
                            }
                            findItemList++;
                        }
                    }
                    findMapData++;
                }
                findNode = findNode->getNext();
            }
            return true;
        }
    } else return false; // not find frequency value in data nodes
}
void BpTree::printFrequentPatterns(set<string> pFrequentPattern) {
    *fout << "{";
    set<string> curPattern = pFrequentPattern;
    for (set<string>::iterator it = curPattern.begin(); it != curPattern.end();) {
        string temp = *it++;
        *fout << temp;
        if (it == curPattern.end()) {
            *fout << "} ";
            break;
        }
        *fout << ", ";
    }
}