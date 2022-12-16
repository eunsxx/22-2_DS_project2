#include "Manager.h"
using namespace std;

int main() {
    Manager manager(2, 3);
    manager.run("command.txt");
    return 0;
}