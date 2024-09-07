#include "dpll_optimize.h"
#include <string>
#include <ctime>

using namespace std;

int main() {
    string filename = "D:\\Dprogram\\Code\\sudoku\\cnf\\2.cnf";
    std::cout << "will create\n";
    INFO info;
    std::cout << "Parsing " << filename << std::endl;
    info.parserCnf(filename);
    std::cout << "will init\n";
    info.init();
    std::cout << "will solve\n";
    clock_t start = clock();
    info.solve();
    clock_t end = clock();
    std::cout << "will print\n";
    info.printAnswer();
    std::cout << "will check\n";
    info.checkAnswer();
    cout << "Time: " << (double)(end - start) << "ms\n";
    return 0;
}