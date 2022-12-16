#include "FPGrowth.h"

FPGrowth::~FPGrowth() {
    frequencyPatterns.clear();
    flog.close();
} // destructor

void FPGrowth::createFPtree(FPNode *root, HeaderTable *table, list<string> item_array, int frequency) {
    FPNode* curr = root;
    list<string>::iterator iter = item_array.begin();
    while (iter != item_array.end()) {
        FPNode* newNode = new FPNode;
        newNode->setNext(nullptr);
        if (curr->getChildrenNode(*iter) == nullptr) { // if children not exist in curr
            curr->pushchildren(*iter, newNode); // create new childeren]
            curr->getChildrenNode(*iter)->setParent(curr); // parent setting
            curr = curr->getChildrenNode(*iter); // curr = current product node
            curr->updateFrequency(frequency); // frequency = 1;
            connectNode(table, *iter, newNode);
        } else {
            curr = curr->getChildrenNode(*iter); // curr = existed product children node
            curr->updateFrequency(frequency); // frequency + 1
        }
        iter++;
    }

}

void FPGrowth::connectNode(HeaderTable *table, string item, FPNode *node) {
    map<string, FPNode*> dt = table->getdataTable(); // dt = data table
    map<string, FPNode*>::iterator it = dt.find(item); // it = find item in data table
    FPNode* curr = it->second; // curr = find item's pointer in data table
    while (curr->getNext() != nullptr) { // Finding last Node of current product pointer in the data table
        curr = curr->getNext();
    }
    curr->setNext(node); // curr is last pointer of data table, curr->next = node.
}

bool FPGrowth::contains_single_path(FPNode *pNode) {
    if (pNode->getChildren().size() == 0) return true;
    else if (pNode->getChildren().size() > 1) return false;
    return contains_single_path(pNode->getChildren().begin()->second);
}

// map<set<string>, int> FPGrowth::getFrequentPatterns(HeaderTable *pTable, FPNode *pTree) {
    //TODO find frequent Pattern. it is bonus
	// return;
// };

void FPGrowth::powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* ptr, int depth) {
    if (data.size() == depth) {
        set<string> set;
        set.insert(item);
        for (int i = 0; i < data.size(); i++)
            if (ptr[i] == 1) set.insert(data[i]);
        FrequentPattern->insert(make_pair(set, frequency));
        return;
    }

    ptr[depth] = 1;
    powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);

    ptr[depth] = 0;
    powerSet(FrequentPattern, data, item, frequency, ptr, depth + 1);
}

bool FPGrowth::printList() {
    list<pair<int, string>>::iterator iter;
    if (getHeaderTable()->getdataTable().empty()) return false;
    else {
        getHeaderTable()->descendingIndexTable();  // descending sort by frequency
        list<pair<int, string>> sort_table = getHeaderTable()->getindexTable(); // table point index table
        if (sort_table.empty()) return false; // error code print statement
        *fout << "Item \t Frequency" << endl;
        for (iter = sort_table.begin(); iter != sort_table.end(); iter++) {
            *fout << iter->second << " " << iter->first << endl;
        }
        return true;
    }
}
bool FPGrowth::printTree() {
    map<string, FPNode*> exist = fpTree->getChildren();
    if (exist.empty()) return false; // not exist in fp tree
    table->ascendingIndexTable();
    map<string, FPNode*> dt = table->getdataTable();
    list<pair<int, string>> it = table->getindexTable();
    map<string, FPNode*>::iterator iter = dt.begin();
    list<pair<int, string>>::iterator iter2 = it.begin();
    while(iter2->first < getThreshold()) iter2++;
    *fout << "{StandardItem, Frequency} (Path_Item, Frequency)" << endl;
    while (iter2 != it.end() ) {
        if (iter2->first < getThreshold()) iter2++; // if frequency smaller than threshold, skip
        // print {product, frequency}
        *fout << "{" << iter2->second << ", " << iter2->first << "}" << endl;

        FPNode* curr = table->getNode(iter2->second); // curr point founded product in the data table
        FPNode* item = curr;
        while (item->getNext() != nullptr) { // next product, and print path to root
            item = item->getNext(); // origin curr is empty node.
            curr = item; // curr is next node in data table
            map<string, FPNode *> m;
            map<string, FPNode *>::iterator mter;
            while (curr->getParent() != nullptr) { // path to root
                FPNode *temp = curr;
                m = curr->getParent()->getChildren(); // Many Children nodes may exist
                mter = m.begin();
                while (mter != m.end()) { // so, find nodes in many children nodes
                    if (mter->second == temp) {
                        *fout << "(" << mter->first << ", " << temp->getFrequency() << ") ";
                    }
                    mter++;
                }
                curr = curr->getParent();
            }
            // if curr is root
            *fout << endl;
        }
        iter2++;
    }
    return true;
}
void FPGrowth::saveFrequentPatterns(){
    // TODO save Frequent Patterns
}
