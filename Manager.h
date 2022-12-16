#include "FPGrowth.h"
#include "BpTree.h"

class Manager{
private:
    char* cmd;
    FPGrowth* fpgrowth;
    BpTree* bptree;

public:
    Manager(int threshold, int bpOrder) { // constructor
        fpgrowth = new FPGrowth(&flog, threshold);
        bptree = new BpTree(&flog, bpOrder);
    }

    ~Manager() { //destructor
        if (fin.is_open())
            fin.close();

        if (flog.is_open())
            flog.close();
    }

    ifstream fin;
    ofstream flog;


    void run(const char* command);
    bool LOAD();
    bool BTLOAD();

    bool PRINT_ITEMLIST();
    bool PRINT_FPTREE();

    bool PRINT_RANGE(string item, int start, int end);
    bool PRINT_CONFIDENCE(string item, double rate);
    bool PRINT_BPTREE(string item, int min_frequency);


    void printErrorCode(int n);
    void printSuccessCode();

};