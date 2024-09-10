#ifndef DPLL_POINTER_H
#define DPLL_POINTER_H

#include <iostream>
#include <cstring>

#define VAR_MAXN 110000
#define CLAUSE_MAXN 220000

struct STACK {
    int stack[CLAUSE_MAXN]{};
    int top;
    STACK() : top(-1) {}
    ~STACK() = default;

    void push(const int var) { stack[++top] = var; } 
    int pop() { return stack[top--]; }
    bool empty() { return top == -1; }
    void clear() { top = -1; }
};

struct NODE {
    int clauseID, var;
    struct NODE *l, *r, *t, *b; // left, right, top, bottom
    NODE():clauseID(0), var(0), l(nullptr), r(nullptr), t(nullptr), b(nullptr) {}
    ~NODE() = default;
};

struct OVERVIEW {
    int clauseCount, boolVarCount, clauseCountBackup, literalCount;
    bool solved, empty;
    int exist[CLAUSE_MAXN]{};
    int assign[VAR_MAXN]{};
    struct NODE* clauseNode[CLAUSE_MAXN]{};
    struct NODE* varNode[VAR_MAXN]{};
    struct NODE** allNode;
    STACK unit, modify;
    int ID2Pos[VAR_MAXN]{}, Pos2ID[VAR_MAXN]{};

    OVERVIEW() : clauseCount(0), boolVarCount(0), clauseCountBackup(0), solved(false), empty(false) {
        memset(assign, 0, sizeof(assign));
        memset(exist, 0, sizeof(exist));
        for (auto& i : clauseNode) 
            i = nullptr;
        for (auto& i : varNode)
            i = nullptr;
        for (int i = 0; i < VAR_MAXN; i++) {
            ID2Pos[i] = i;
            Pos2ID[i] = i;
        }
    }

    ~OVERVIEW() {
        for (int i = 0; i < literalCount; i++)
            delete allNode[i];
        delete[] allNode;
    }

    void parserCnfFile(const std::string& filename);
    void initNodes();
    void printByClause();
    void printByVar();

    int negatedVar(int var); 
    void assignVar(int var);
    void reassignVar(int var);

    void removeClauseByVar(int var);
    void restoreClauseByVar(int var);
    void removeLiteralFromClause(int var);
    void restoreLiteralToClause(int var);
    void restoreToTop(int top);

    int chooseOneVar();
    void actionForVar(int var);
    void solve();
    void printAssign();
    void checkAssign();

    void clearAllUnit() {
        for (int i = 1; i <= clauseCount; i++) {
            struct NODE* now = clauseNode[i];
            if (now->r == nullptr) {
                unit.push(i);
            }
        }
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

    void pureDocument() {
        int varCount[VAR_MAXN];
        for (int i = 1; i <= 2 * boolVarCount; i++) {
            struct NODE* now = varNode[i];
            while (now) {
                int clauseID = now->clauseID;
                if (exist[clauseID] == 0)
                    varCount[i] ++;
                now = now->b;
            }
        }
        for (int i = 1; i <= boolVarCount; i++) {
            if (varCount[i] == 0 && varCount[i + boolVarCount] != 0 ||
                varCount[i] != 0 && varCount[i + boolVarCount] == 0) {
                    int idx = varCount[i] != 0 ? i : i + boolVarCount;
                    std::cout << "pure document: " << idx << std::endl;
                    // struct NODE* now = varNode[idx];
                    // while (now) {
                    //     int clauseID = now->clauseID;
                    //     if (exist[clauseID] == 0) {
                    //         unit.push(clauseID);
                    //         break;
                    //     }
                    //     now = now->b;
                    // }           
                }
        }   
    }

    void pureLiteral() {
        int varCount[VAR_MAXN];
        for (int i = 1; i <= 2 * boolVarCount; i++) {
            struct NODE* now = varNode[i];
            while (now) {
                int clauseID = now->clauseID;
                if (exist[clauseID] == 0)
                    varCount[i] ++;
                now = now->b;
            }
        }        
        // for (int i = 1; i <= boolVarCount; i++) {
        //     std::cout << i << " " << varCount[i] << " " << -i << " " << varCount[i + boolVarCount] << std::endl;
        // }
        for (int i = 1; i <= boolVarCount; i++) {
            if (varCount[i] == 0 && varCount[i + boolVarCount] != 0 ||
                varCount[i] != 0 && varCount[i + boolVarCount] == 0) {
                    int idx = varCount[i] != 0 ? i : i + boolVarCount;
                    struct NODE* now = varNode[idx];
                    while (now) {
                        int clauseID = now->clauseID;
                        if (exist[clauseID] == 0) {
                            unit.push(clauseID);
                            break;
                        }
                        now = now->b;
                    }           
                }
        }
    }

    void solveProblem() {
        do {
            clearAllUnit();
            pureLiteral();
        } while (solved == false && empty == false && unit.empty() == false);
        // clearAllUnit();
        // pureLiteral();
        if (solved == true) return;
        if (empty == true) {
            std::cout << "when clear all unit, the problem is UNSAT\n";
            return;
        }
        solve();
    }

    void analyse() {
        int varCount[VAR_MAXN];
        memset(varCount, 0, sizeof(varCount));
        for (int i = 1; i <= clauseCount; i++) {
            struct NODE* now = clauseNode[i];
            if (now->r == nullptr) {
                std::cout << " this is a unit\n";
            }
            while (now) {
                varCount[now->var]++;
                now = now->r;
            }
        }
        for (int i = 1; i <= boolVarCount; i++) {
            std::cout << i << " " << varCount[i] << " " << -i << " " << varCount[i + boolVarCount] << std::endl;
        }
        for (int i = 1; i <= boolVarCount; i++) {
            if (varCount[i] == 0 && varCount[i + boolVarCount] != 0 ||
                varCount[i] != 0 && varCount[i + boolVarCount] == 0) {
                    if (varCount[i] == 0) std::cout << i << " all\n";
                    else std::cout << -i << " all\n";
                }
        }
        
    }
};

#endif