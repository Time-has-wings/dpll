#include "dpll-pointer.h"
#include <string>
#include <ctime>

using namespace std;

int num;

int main() {
    std::cin >> num;
    
    OVERVIEW* overview = new OVERVIEW();
    string filename = "D:\\Dprogram\\Code\\sudoku\\cnf\\" + to_string(num) + ".cnf";
    std::cout << "will create\n";
    std::cout << "Parsing " << filename << std::endl;
    overview->parserCnfFile(filename);
    std::cout << "will init\n";
    overview->initNodes();
    std::cout << "will print\n";
    std::cout << "will solve\n";
    clock_t start = clock();
    overview->solveProblem();
    clock_t end = clock();
    if (overview->solved) {
        std::cout << "will print\n";
        // overview.printAssign();
        std::cout << "will check\n";
        overview->checkAssign();
    }
    cout << "Time: " << (double)(end - start) << "ms\n";

    OVERVIEW* over = new OVERVIEW();
    string filename2 = "D:\\Dprogram\\Code\\sudoku\\cnf\\" + to_string(num) + ".cnf";
    std::cout << "Parsing " << filename2 << std::endl;
    over->parserCnfFile(filename2);
    std::cout << "will init\n";
    over->initNodes();
    std::cout << "will solve\n";
    clock_t start2 = clock();
    over->solve();
    clock_t end2 = clock();
    if (over->solved) {
        std::cout << "will print\n";
        // over->printAssign();
        std::cout << "will check\n";
        over->checkAssign();
    }
    cout << "Time: " << (double)(end2 - start2) << "ms\n";
    return 0;
}