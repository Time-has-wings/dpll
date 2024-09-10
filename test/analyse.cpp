#include "dpll-pointer.h"

using namespace std;

int main() {
    for (int i = 1; i <= 12; i++) {
        std::cout << "==================================================\n";
        OVERVIEW* overview = new OVERVIEW();
        string filename = "D:\\Dprogram\\Code\\sudoku\\cnf\\" + to_string(i) + ".cnf";
        std::cout << "Parsing " << filename << std::endl;
        overview->parserCnfFile(filename);
        std::cout << "will init\n";
        overview->initNodes();
        std::cout << "will analyse\n";
        overview->analyse();
        delete overview;
        std::cout << "==================================================\n";
    }
    return 0;
}