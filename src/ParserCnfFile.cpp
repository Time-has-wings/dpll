#include "ParserCnfFile.h"

Clause* createClause() {
    Clause* clause = new Clause;
    int literal;
    while (std::cin >> literal) {
        if (literal == 0) 
            break;
        clause->literalCount++;
        Literal* newLiteral = new Literal;
        newLiteral->value = abs(literal);
        newLiteral->negated = literal < 0;
        newLiteral->next = clause->literals;
        clause->literals = newLiteral;
    }
    return clause;
}

void printClauses(Clause* root) {
    Clause* current = root;
    while (current != nullptr) {
        Literal* currentLiteral = current->literals;
        while (currentLiteral != nullptr) {
            std::cout << (currentLiteral->negated ? "-" : "") << currentLiteral->value << " ";
            currentLiteral = currentLiteral->next;
        }
        std::cout << "0\n";
        current = current->next;
    }
}

void parseCnfFile(std::string filename) {
    freopen(filename.c_str(), "r", stdin);
    int boolVarCount, clauseCount;
    char op;
    while (std::cin >> op) {
        if (op == 'p') {
            // std::string CNFRubbish;
            // std::cin >> CNFRubbish >> boolVarCount >> clauseCount;
            break;
        }
        else {
            std::cin.ignore(256, '\n');
        }
    }
    Clause* root, *tail = nullptr;
    for (int i = 0; i < clauseCount; i++) {
        Clause* newClause = createClause();
        if (root == nullptr) {
            root = newClause;
            tail = newClause;
        }
        else {
            tail->next = newClause;
            tail = newClause;
        }
    }
    // return root;
}