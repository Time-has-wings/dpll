#include "parser.h"

static void addLiteralToClause(LITERAL* literal, CLAUSE* clause) {
    if (clause->head_literal == nullptr) {
        clause->head_literal = literal;
        clause->tail_literal = literal;
        clause->tail_literal->next = nullptr;
    } else {
        clause->tail_literal->next = literal;
        clause->tail_literal = literal;
        clause->tail_literal->next = nullptr;
    }
}

static void addClauseToRoot(CLAUSE* clause, ROOT* root) {
    if (root->head_clause == nullptr) {
        root->head_clause = clause;
        root->tail_clause = clause;
        root->tail_clause->next = nullptr;
    } else {
        root->tail_clause->next = clause;
        root->tail_clause = clause;
        root->tail_clause->next = nullptr;
    }
}

ROOT* parserCnfFile(std::string filename) {
    freopen(filename.c_str(), "r", stdin);
    char op;
    int bool_val_count, clause_count;
    std::string rubbish;
    while (true) {
        std::cin >> op;
        if (op != 'c') {
            std::cin >> rubbish >> bool_val_count >> clause_count;
            break;
        }
        else {
            std::getline(std::cin, rubbish);
        }
    }
    ROOT* root = new ROOT();
    root->bool_var_count = bool_val_count;
    for (int i = 0; i < clause_count; i++) {
        CLAUSE* clause = new CLAUSE();
        int bool_var;
        while (true) {
            std::cin >> bool_var;
            if (bool_var == 0) break;
            LITERAL* literal = new LITERAL();
            literal->setBoolVar(bool_var);
            addLiteralToClause(literal, clause);
        }
        addClauseToRoot(clause, root);
    }
    return root;
}

static void printClause(CLAUSE* clause) {
    LITERAL* literal = clause->head_literal;
    while (literal != nullptr) {
        std::cout << literal->bool_var << " ";
        literal = literal->next;
    }
    std::cout << '\n';
}

void printRoot(ROOT* root) {
    std::cout << "printRoot\n";
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        printClause(clause);
        clause = clause->next;
    }
}

static CLAUSE* backupClause(CLAUSE* clause) {
    CLAUSE* new_clause = new CLAUSE();
    LITERAL* literal = clause->head_literal;
    while (literal != nullptr) {
        LITERAL* new_literal = new LITERAL();
        new_literal->setBoolVar(literal->bool_var);
        addLiteralToClause(new_literal, new_clause);
        literal = literal->next;
    }
    return new_clause;
}

ROOT* backupRoot(ROOT* root) {
    ROOT* new_root = new ROOT();
    new_root->bool_var_count = root->bool_var_count;
    new_root->solved = root->solved;
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        CLAUSE* new_clause = backupClause(clause);
        addClauseToRoot(new_clause, new_root);
        clause = clause->next;
    }
    return new_root;
}

static bool isUnitClause(CLAUSE* clause) {
    return clause->head_literal == clause->tail_literal && clause->head_literal != nullptr;
}

static bool isEmptyClause(CLAUSE* clause) {
    return clause->head_literal == nullptr;
}

static bool isEmptyRoot(ROOT* root) {
    return root->head_clause == nullptr;
}

static void releaseClause(CLAUSE* clause) {
    LITERAL* literal = clause->head_literal;
    while (literal != nullptr) {
        LITERAL* tmp = literal;
        literal = literal->next;
        delete tmp;
    }
    delete clause;
}

void releaseRoot(ROOT* root) {
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        CLAUSE* tmp = clause;
        clause = clause->next;
        releaseClause(tmp);
    }
    delete root;
}

static void removeOneLiteral(CLAUSE* clause, LITERAL* literal) {
    if (clause->head_literal == literal) {
        clause->head_literal = literal->next;
        if (clause->tail_literal == literal) 
            clause->tail_literal = nullptr;
    } else {
        LITERAL* search = clause->head_literal;
        while (search != nullptr && search->next != nullptr && search->next != literal) 
            search = search->next;
        assert(search != nullptr && search->next != nullptr);
        search->next = search->next->next;
    }
    delete literal;
}

static void removeOneClause(ROOT* root, CLAUSE* clause) {
    if (root->head_clause == clause) {
        root->head_clause = clause->next;
        if (root->tail_clause == clause) 
            root->tail_clause = nullptr;
    } else {
        CLAUSE* search = root->head_clause;
        while (search != nullptr && search->next != nullptr && search->next != clause) 
            search = search->next;
        assert(search != nullptr && search->next != nullptr);
        search->next = search->next->next;
    }
    releaseClause(clause);
}

CLAUSE* findOneUnitClause(ROOT* root) {
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        if (isUnitClause(clause)) return clause;
        clause = clause->next;
    }
    return nullptr;
}

static ACTIONTYPE actionType(CLAUSE* clause, int bool_var) {
    LITERAL* literal = clause->head_literal;
    while (literal != nullptr) {
        if (literal->bool_var == bool_var) return REMOVE_CLAUSE;
        else if (literal->bool_var == -bool_var) return REMOVE_LITERAL;
        else literal = literal->next;
    }
    return ACTIONTYPE_NOTHING;
}

SIMPLIFYRESULT simplifyRoot(ROOT* root, int bool_var) {
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        ACTIONTYPE action = actionType(clause, bool_var);
        if (action == REMOVE_CLAUSE) {
            CLAUSE* tmp = clause;
            clause = clause->next;
            removeOneClause(root, tmp);
        } else if (action == REMOVE_LITERAL) {
            LITERAL* literal = clause->head_literal;
            while (literal != nullptr && literal->bool_var != -bool_var) 
                literal = literal->next;
            removeOneLiteral(clause, literal);
            if (isEmptyClause(clause))
                return EMPTY_CLAUSE;
        } else clause = clause->next;
    }
    if (isEmptyRoot(root)) return EMPTY_ROOT;
    else return SIMPLIFYRESULT_NOTHING;
}

void createOneSingleClause(ROOT* root, int bool_var) {
    CLAUSE* clause = new CLAUSE();
    LITERAL* literal = new LITERAL();
    literal->setBoolVar(bool_var);
    literal->next = nullptr;
    clause->head_literal = literal;
    clause->tail_literal = literal;
    clause->next = root->head_clause;
    root->head_clause = clause;
}

void checkAnswer(ROOT* root, int* ans) {
    CLAUSE* clause = root->head_clause;
    while (clause != nullptr) {
        LITERAL* literal = clause->head_literal;
        bool flag = false;
        while (literal != nullptr) {
            if (literal->bool_var = ans[abs(literal->bool_var)] * abs(literal->bool_var)) {
                flag = true;
                break;
            }
            literal = literal->next;
        }
        if (flag == false) {
            printClause(clause); // print the wrong clause
            std::cout << "Wrong Answer\n";
            return;
        }
        clause = clause->next;
    }
    std::cout << "Correct Answer\n";
}