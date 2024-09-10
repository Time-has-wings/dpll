#include "dpll-pointer.h"
#include <cassert>

void OVERVIEW::parserCnfFile(const std::string& filename) {
    freopen(filename.c_str(), "r", stdin);
    char op;
    std::string rubbish;
    while (true) {
        std::cin >> op;
        if (op != 'c') {
            std::cin >> rubbish >> this->boolVarCount >> this->clauseCount;
            break;
        }
        else {
            std::getline(std::cin, rubbish);
        }
    }
    clauseCountBackup = clauseCount;
    for (int i = 1; i <= this->clauseCount; i++) {
        int var;
        struct NODE* prev = nullptr;
        while (true) {
            std::cin >> var;
            if (var == 0) break;
            struct NODE* now = new NODE();
            now->var = var > 0 ? var : boolVarCount - var; // 索引变换
            now->clauseID = i;
            literalCount ++;
            if (clauseNode[i] == nullptr) {
                clauseNode[i] = now;
                prev = now;
            }
            else {
                prev->r = now;
                now->l = prev;
                prev = now;
            }
        }
    }
}

void OVERVIEW::initNodes() {
    struct NODE* tail[2 * boolVarCount + 1];
    for (int i = 1; i <= 2 * boolVarCount; i++) 
        tail[i] = nullptr;
    allNode = new NODE*[literalCount];
    int idx = 0;
    for (int i = 1; i <= clauseCount; i++) {
        struct NODE* now = clauseNode[i];
        while (now) {
            allNode[idx++] = now;
            if (varNode[now->var] == nullptr) {
                varNode[now->var] = now;
                tail[now->var] = now;
            }
            else {
                tail[now->var]->b = now;
                now->t = tail[now->var];
                tail[now->var] = now;
            }
            now = now->r;
        }
    }
}

void OVERVIEW::printByClause() {
    for (int i = 1; i <= clauseCount; i++) {
        struct NODE* now = clauseNode[i];
        while (now) {
            if (now->var > boolVarCount) std::cout << "-" << now->var - boolVarCount << " ";
            else std::cout << now->var << " ";
            now = now->r;
        }
        std::cout << std::endl;
    }
}

void OVERVIEW::printByVar() {
    for (int i = 1; i <= 2 * boolVarCount; i++) {
        struct NODE* now = varNode[i];
        if (i > boolVarCount) std::cout << "-" << i - boolVarCount << ": ";
        else std::cout << i << ": ";
        while (now) {
            std::cout << now->clauseID << " ";
            now = now->b;
        }
        std::cout << std::endl;
    }
}

int OVERVIEW::negatedVar(int var) {
    return var > boolVarCount ? var - boolVarCount : var + boolVarCount;
}

void OVERVIEW::assignVar(int var) {
    modify.push(var);
    if (var > boolVarCount) assign[var - boolVarCount] = -1;
    else assign[var] = 1;
}

void OVERVIEW::reassignVar(int var) {
    const int idx = var > boolVarCount ? var - boolVarCount : var;
    assign[idx] = 0;
}

// void OVERVIEW::removeClauseByVar(int var) {
//     struct NODE* now = varNode[var];
//     while (now) {
//         int clauseID = now->clauseID;
//         assert(exist[clauseID] == 0);
//         struct NODE* literal = clauseNode[clauseID];
//         while (literal) {
//             if (literal->var != var && literal->var != negatedVar(var)) {
//                 int literalVar = literal->var;
//                 struct NODE* top = literal->t;
//                 struct NODE* bottom = literal->b;
//                 if (top == nullptr && bottom == nullptr) {
//                     varNode[literalVar] = nullptr;
//                 }
//                 else if (top != nullptr && bottom == nullptr) {
//                     top->b = nullptr;
//                 }
//                 else if (top == nullptr && bottom != nullptr) {
//                     bottom->t = nullptr;
//                     varNode[literalVar] = bottom;
//                 }
//                 else {
//                     top->b = bottom;
//                     bottom->t = top;
//                 }
//             }
//             literal = literal->r;
//         }
//         int pos = ID2Pos[clauseID];
//         Pos2ID[pos] = Pos2ID[clauseCount];
//         ID2Pos[Pos2ID[clauseCount]] = pos;
//         exist[clauseID] = var;
//         clauseCount --;
//         now = now->b;
//     }
//     if (clauseCount == 0)
//         solved = true;
// }

// void OVERVIEW::restoreClauseByVar(int var) {
//     struct NODE* now = varNode[var];
//     while (now) {
//         int clauseID = now->clauseID;
//         assert(exist[clauseID] == var);

//         struct NODE* literal = clauseNode[clauseID];
//         while (literal) {
//             if (literal->var != var && literal->var != negatedVar(var)) {
//                 int literalVar = literal->var;
//                 struct NODE* top = literal->t;
//                 struct NODE* bottom = literal->b;
//                 if (top == nullptr && bottom == nullptr) {
//                     varNode[literalVar] = literal;
//                 }
//                 else if (top != nullptr && bottom == nullptr) {
//                     top->b = literal;
//                 }
//                 else if (top == nullptr && bottom != nullptr) {
//                     bottom->t = literal;
//                     varNode[literalVar] = literal;
//                 }
//                 else {
//                     top->b = literal;
//                     bottom->t = literal;
//                 }
//             }
//             literal = literal->r;
//         }

//         exist[clauseID] = 0;
//         clauseCount ++;
//         Pos2ID[clauseCount] = clauseID;
//         ID2Pos[clauseID] = clauseCount;
//         now = now->b;
//     }
// }

void OVERVIEW::removeClauseByVar(int var) {
    struct NODE* now = varNode[var];
    while (now) {
        int clauseID = now->clauseID;
        if (exist[clauseID] == 0) {
            int pos = ID2Pos[clauseID];
            Pos2ID[pos] = Pos2ID[clauseCount];
            ID2Pos[Pos2ID[clauseCount]] = pos;
            exist[clauseID] = var;
            clauseCount --;
        }
        now = now->b;
    }
    if (clauseCount == 0)
        solved = true;
}

void OVERVIEW::restoreClauseByVar(int var) {
    struct NODE* now = varNode[var];
    while (now) {
        int clauseID = now->clauseID;
        if (exist[clauseID] == var) {
            exist[clauseID] = 0;
            clauseCount ++;
            Pos2ID[clauseCount] = clauseID;
            ID2Pos[clauseID] = clauseCount;
        }
        now = now->b;
    }
}

void OVERVIEW::removeLiteralFromClause(int var) {
    struct NODE* now = varNode[var];
    while (now) {
        int clauseID = now->clauseID;
        if (exist[clauseID] == 0) {
                struct NODE* prev = now->l;
                struct NODE* next = now->r;
                if (prev == nullptr && next == nullptr) {
                    empty = true;
                    clauseNode[clauseID] = nullptr;
                }
                else if (prev != nullptr && next == nullptr) {
                    prev->r = nullptr;
                    if (clauseNode[clauseID]->r == nullptr)
                        unit.push(clauseID);
                }
                else if (prev == nullptr && next != nullptr) {
                    next->l = nullptr;
                    clauseNode[clauseID] = next;
                    if (clauseNode[clauseID]->r == nullptr)
                        unit.push(clauseID);
                }
                else {
                    prev->r = next;
                    next->l = prev;
                }
        }
        now = now->b;
    }
}

void OVERVIEW::restoreLiteralToClause(int var) {
    struct NODE* now = varNode[var];
    while (now) {
        int clauseID = now->clauseID;
        if (exist[clauseID] == 0) {
            struct NODE* prev = now->l;
            struct NODE* next = now->r;
            if (prev == nullptr && next == nullptr) {
                clauseNode[clauseID] = now;
            }
            else if (prev != nullptr && next == nullptr) {
                prev->r = now;
            }
            else if (prev == nullptr && next != nullptr) {
                next->l = now;
                clauseNode[clauseID] = now;
            }
            else {
                prev->r = now;
                next->l = now;
            }
        }
        now = now->b;
    }
}

void OVERVIEW::restoreToTop(const int top) {
    empty = false;
    unit.clear();
    while (modify.top > top) {
        const int var = modify.pop();
        reassignVar(var);
        restoreClauseByVar(var);
        restoreLiteralToClause(negatedVar(var));
    }
}

int OVERVIEW::chooseOneVar() {
    assert(clauseCount != 0);
    if (clauseCount != 0) {
        int clauseID = Pos2ID[1];
        assert(exist[clauseID] == 0);
        return clauseNode[clauseID]->var;
    }
    return 0;
}

void OVERVIEW::actionForVar(int var) {
    int top = modify.top;
    assignVar(var);
    removeClauseByVar(var);
    removeLiteralFromClause(negatedVar(var));
    if (empty == false) {
        while (!unit.empty()) {
            int clauseID = unit.pop();
            if (exist[clauseID] == 0 && clauseNode[clauseID] != nullptr) {
                int qvar = clauseNode[clauseID]->var;
                assignVar(qvar);
                removeClauseByVar(qvar);
                removeLiteralFromClause(negatedVar(qvar));
                if (empty) break;
            }
        }
    }
    if (solved == false && empty == false)
        solve();
    if (solved == true) return;
    else restoreToTop(top);   
}

void OVERVIEW::solve() {
    int var = chooseOneVar();
    if (var == 0) return;
    var = negatedVar(var); // NOTE
    actionForVar(var);
    if (solved == true) return;
    actionForVar(negatedVar(var));
}

void OVERVIEW::printAssign() {
    for (int i = 1; i <= boolVarCount; i++) {
        if (assign[i] != -1) std::cout << i << std::endl;
        else std::cout << "-" << i << std::endl;
    }
    std::cout << std::endl;
}

void OVERVIEW::checkAssign() {
    bool flag[clauseCountBackup + 1];
    memset(flag, false, sizeof(flag));
    for (int i = 0; i < literalCount; i++) {
        struct NODE* now = allNode[i];
        if (now->var > boolVarCount && assign[now->var - boolVarCount] == -1) 
            flag[now->clauseID] = true;
        else if (now->var <= boolVarCount && assign[now->var] == 1) 
            flag[now->clauseID] = true;
    }
    for (int i = 1; i <= clauseCountBackup; i++) 
        if (flag[i] == false) {
            std::cout << "Error: " << i << std::endl;
            return;
        }
    std::cout << "Correct" << std::endl;
}