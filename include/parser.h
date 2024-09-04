#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

struct LITERAL {
    int bool_var;
    bool negated;
    struct LITERAL* next;

    void setBoolVar(int var) {
        bool_var = var;
        negated = var < 0;
    }
};

struct CLAUSE {
    struct LITERAL* head_literal;
    struct LITERAL* tail_literal;
    struct CLAUSE* next;

    CLAUSE(): head_literal(nullptr), tail_literal(nullptr), next(nullptr) {}
    ~CLAUSE() = default;
};

struct ROOT {
    struct CLAUSE* head_clause;
    struct CLAUSE* tail_clause;
    int bool_var_count;
    bool solved;

    ROOT(): head_clause(nullptr), tail_clause(nullptr), bool_var_count(0), solved(false) {}
    ~ROOT() = default;
};

enum ACTIONTYPE {
    REMOVE_LITERAL = 0,
    REMOVE_CLAUSE,
    ACTIONTYPE_NOTHING
};

enum SIMPLIFYRESULT {
    EMPTY_CLAUSE = 0,
    EMPTY_ROOT,
    SIMPLIFYRESULT_NOTHING
};

ROOT* parserCnfFile(std::string filename);
void printRoot(ROOT* root);
ROOT* backupRoot(ROOT* root);
CLAUSE* findOneUnitClause(ROOT* root);
SIMPLIFYRESULT simplifyRoot(ROOT* root, int bool_var);
void createOneSingleClause(ROOT* root, int bool_var);
void releaseRoot(ROOT* root);
void checkAnswer(ROOT* root, int* ans);

#endif