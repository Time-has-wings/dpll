#include "dpll.h"

using namespace std;

int main() {
    string filename = "D:\\Dprogram\\Code\\sudoku\\cnf\\2.cnf";
    ROOT* root = parserCnfFile(filename);
    cout << "start dpll" << endl;
    basicDpll(root);
    cout << "over dpll" << endl;
    return 0;
}