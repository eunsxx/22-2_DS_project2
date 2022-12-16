#include "Manager.h"
#include <sstream>

void Manager::run(const char* command) {
    fin.open("command.txt"); // TODO 리눅스로 변경할 때 경로 다시 바꿔줘야함
    flog.open("log.txt"); // TODO 리눅스 변경 시 경로 다시.
    if(!fin) {
        cout << "command.txt File Open Error" << endl;
        return;
    }
    if (!flog) {
        cout << "log.txt File Open Error" << endl;
        return;
    }

    string line, cmd;
    while (!fin.eof()) {
        getline(fin, line);
        istringstream iss(line);
        string str_buf[4];
        int i = 0;
        while (getline(iss, str_buf[i], '\t')) i++; // cut words based on white space

        cmd = str_buf[0];
        if (cmd == "LOAD") {
            flog << "========LOAD========" << endl;
            if(LOAD()) printSuccessCode();
            else printErrorCode(100);
        }
        else if (cmd == "BTLOAD") {
            flog << "========BTLOAD========" << endl;
            if (BTLOAD()) printSuccessCode();
            else printErrorCode(200);
        }
        else if (cmd == "PRINT_ITEMLIST") {
            flog << "========PRINT_ITEMLIST=======" << endl;
            if(PRINT_ITEMLIST()) flog << "=======================" << endl << endl;
            else printErrorCode(300);
        }
        else if (cmd == "PRINT_FPTREE") {
            flog << "========PRINT_FPTREE=======" << endl;
            if(PRINT_FPTREE()) flog << "=======================" << endl << endl;
            else printErrorCode(400);
        }
        else if (cmd == "PRINT_BPTREE") {
            flog << "========PRINT_BPTREE=======" << endl;
            string item;
            int min_frequency;
            item = str_buf[1];
            min_frequency = stoi(str_buf[2]);
            if(PRINT_BPTREE(item, min_frequency)) continue;
            else printErrorCode(500);
        }
        else if (cmd == "PRINT_CONFIDENCE") {
            flog << "========PRINT_CONFIDENCE=======" << endl;
            string item;
            double rate;
            item = str_buf[1];
            rate = stod(str_buf[2]);
            if (PRINT_CONFIDENCE(item, rate)) flog << "=======================" << endl << endl;
            else printErrorCode(600);
        }
        else if (cmd == "PRINT_RANGE") {
            flog << "========PRINT_RANGE=======" << endl;
            string item;
            int start, end;
            item = str_buf[1];
            start= stoi(str_buf[2]);
            end= stoi(str_buf[3]);
            if (PRINT_RANGE(item, start, end)) flog << "=======================" << endl << endl;
            else printErrorCode(700);
        } else if (cmd == "EXIT") {
            flog << "========EXIT=======" << endl;
            printSuccessCode();
            return;
        }
        else cout << "wrong command" << endl;
    }
    fin.close();
    return;
}

bool Manager::LOAD() {
    ifstream file; // file is 'market.txt'
    file.open("market.txt");
    string line, str_buf[10000];
    list<pair<int, string>>::iterator iter;

    // create header table
    if (!file.is_open()) {
        cout << "Market.txt file open error" << endl;
        return false; // not find .txt file
    } else {
        while (!file.eof()) {
            getline(file, line);
            istringstream iss(line);
            int i = 0;
            while (getline(iss, str_buf[i], '\t')) { // cut words based on white space
                int findFrequency = fpgrowth->item_frequency(str_buf[i]);
                findFrequency++; // findFrequency = findFrequency + 1
                fpgrowth->createTable(str_buf[i], findFrequency); // update header table
                i++;
            }
        }
        file.close();

        file.open("market.txt");
        while (!file.eof()) {
            list<string> item_arr; // transaction sort in arr
            getline(file, line);
            istringstream iss(line);
            int i = 0;
            while (getline(iss, str_buf[i], '\t')) i++;
            fpgrowth->getHeaderTable()->descendingIndexTable(); // sort index table in descending
            list<pair<int, string>> table = fpgrowth->getHeaderTable()->getindexTable();
            iter = table.begin(); // iter point index table
            while (iter->first >= fpgrowth->getThreshold() && iter != table.end()) { // when frequency >= threshold
                for (int j = 0; j < i; j++)
                    if (iter->second == str_buf[j]) {
                        item_arr.push_back(str_buf[j]); // sort transaction based on frequency
                    }
                iter++;
            }
            list<string>::iterator iter_item;
            fpgrowth->createFPtree(fpgrowth->getTree(), fpgrowth->getHeaderTable(), item_arr, 1);
        }
    }
    return true;

}
bool Manager::BTLOAD() {
    // 'result.txt' file open
    ifstream file;
    file.open("result.txt");
    string line, str_buf[1000];

    // file read
    int count = 0;
    if(file.is_open()) {
        while (!file.eof()) {
            count++;
            getline(file, line);
            int i = 0;
            istringstream iss(line);
            set<string> item;
            int frequency;
            while (getline(iss, str_buf[i], '\t')) i++; // cut words based on white space
            frequency = stoi(str_buf[0]);
            for (int j = 1; j < i; j++) {
                item.insert(str_buf[j]);
            }
            bptree->Insert(frequency, item);
        }
        return true;
    } else return false;
}
bool Manager::PRINT_ITEMLIST() {
    if (fpgrowth->printList()) return true;
    else return false;
}
bool Manager::PRINT_FPTREE() {
    if (fpgrowth->printTree()) return true;
    else return false;
}
bool Manager::PRINT_BPTREE(string item, int min_frequency) {
    if (bptree->getRoot() == nullptr) return false;
    if (bptree->printFrequency(item, min_frequency)) {
        flog << "=======================" << endl << endl;
        return true;
    }
    else return false;
}
bool Manager::PRINT_CONFIDENCE(string item, double rate) {
    if (bptree->getRoot() == nullptr) return false;
    double item_count = 0;
    BpTreeNode* curr = bptree->getRoot();
    while (curr->getIndexMap() != nullptr) curr = curr->getMostLeftChild();
    while(curr != nullptr) {
        map<int, FrequentPatternNode *>::iterator currData = curr->getDataMap()->begin();
        while (currData != curr->getDataMap()->end()) {
            multimap<int, set<string>> pattern = currData->second->getList();
            multimap<int, set<string>>::iterator itemList = pattern.begin();
            while (itemList != pattern.end()) {
                set<string>::iterator findItem = itemList->second.begin();
                while (findItem != itemList->second.end()) {
                    if (*findItem == item) item_count++;
                    findItem++;
                }
                itemList++;
            }
            currData++;
        }
        curr = curr->getNext();
    }
    if (bptree->printConfidence(item, item_count, rate)) return true;
    else return false;
}
bool Manager::PRINT_RANGE(string item, int start, int end) {
    if (bptree->printRange(item, start, end)) return true;
    else return false;
}

void Manager::printErrorCode(int n) {				//ERROR CODE PRINT
    flog << "ERROR " << n << " " << endl;
    flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode() {//SUCCESS CODE PRINT
    flog << "Success" << endl;
    flog << "=======================" << endl << endl;
}

