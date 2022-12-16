#include "HeaderTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

class FPGrowth{
private:
    int threshold;
    FPNode* fpTree;
    HeaderTable* table;
    map<set<string>, int> frequencyPatterns;
    ofstream *fout; // log file
    ofstream flog; // result file

public:
    FPGrowth(ofstream *fout, int threshold) { // constructor
        this->threshold = threshold; // set threshold
        flog.open("result.txt", ios::app); // result.txt file open. result.txt = flog
        flog.setf(ios::fixed); // fixed point set
        this->fout = fout;
        fpTree = new FPNode; // fpTree is FPNode. FPNode has value of parent, next, frequency, children(map)
        table = new HeaderTable; // table is HeaderTable. HeaderTable has value of indexTable(list), dataTable(map)
        this->fout = fout; // fout = log.txt
    }

    ~FPGrowth(); // destructor
    void createTable(string item, int frequency) { table->insertTable(item, frequency); }
    void createFPtree(FPNode* root, HeaderTable* table, list<string> item_array, int frequency);
    void connectNode(HeaderTable* table, string item, FPNode* node);

    void frequentPatternSetting(){
        table->ascendingIndexTable(); // sort by ascending index
        frequencyPatterns = getFrequentPatterns(table, fpTree);
    }

    map<set<string>, int> getFrequentPatterns(HeaderTable* pTable, FPNode* pTree);
    void powerSet(map<set<string>, int>* FrequentPattern, vector<string> data, string item, int frequency, int* flag, int depth);
    bool contains_single_path(FPNode* pNode);
    int item_frequency(string item) { return table->find_frequency(item); }
    FPNode* getTree() { return fpTree; }
    HeaderTable* getHeaderTable() { return table; }
    int getThreshold() {return threshold; }

    bool printList();
    bool printTree();
    void saveFrequentPatterns();
};

