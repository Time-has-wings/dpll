#ifndef PARSERCNFFILE_H
#define PARSERCNFFILE_H

#include <iostream>
#include <string>
#include <cmath>

struct Literal {
    int value;
    bool negated;
    Literal* next;
};

struct Clause {
    Literal* literals;
    int literalCount;
    Clause* next;
    Clause(): literals(nullptr), literalCount(0), next(nullptr){}
};

Clause* createClause();

void printClauses(Clause* root);

void parseCnfFile(std::string filename);

#endif // PARSERCNFFILE_H