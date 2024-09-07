#ifndef DPLL_OPTIMIZE_H
#define DPLL_OPTIMIZE_H

#include <iostream>
#include <cstring>
#include <cmath>
#include <random>

#define MAXN 10000

struct STACK {
    int stack[MAXN], top;

    void init() {
        top = -1;
    }

    void push(const int var) {
        stack[++top] = var;
    }

    int pop() {
        return stack[top--];
    }
    
    bool empty() {
        return top == -1;
    }

    void clear() {
        top = -1;
    }
};

struct PAIR {
    int first, second;
};

struct PAIR_ARRAY {
    int* idx;
    int* slot;
    int capacity;

    void init(const int capacity_) {
        capacity = capacity_;
        idx = new int[capacity];
        slot = new int[capacity];
    }

    void remove(const int pos) {
        idx[pos] = idx[capacity - 1];
        slot[pos] = slot[capacity - 1];
        capacity--;
    } 

    void add(int idx_, int slot_) {
        idx[capacity] = idx_;
        slot[capacity] = slot_;
        capacity++;
    }

    void set(const int pos, const int idx_, const int slot_) {
        idx[pos] = idx_;
        slot[pos] = slot_;
    }

    PAIR get(const int pos) {
        return PAIR{idx[pos], slot[pos]};
    }
};

struct ELEM {
    int val;
    struct ELEM* next;
};

struct INFO {
    struct ELEM* head;
    struct ELEM* tail;
    int clause_count, bool_var_count, number_count;
    bool empty_clause, empty_root;
    STACK unit, modify;
    int assign[MAXN];
    bool exist[MAXN];
    int invalid[MAXN];
    PAIR_ARRAY clause[MAXN], number[MAXN];
    int clause_capacity[MAXN], number_capacity[MAXN];

    void addElemToInfo(struct ELEM* elem) {
        if (head == nullptr) {
            head = elem;
            tail = elem;
            tail->next = nullptr;
        } 
        else {
            tail->next = elem;
            tail = elem;
            tail->next = nullptr;
        }
    }

    void parserCnf(const std::string& filename) {
        freopen(filename.c_str(), "r", stdin);
        char op;
        std::string rubbish;
        while (true) {
            std::cin >> op;
            if (op != 'c') {
                std::cin >> rubbish >> this->bool_var_count >> this->clause_count;
                break;
            }
            else {
                std::getline(std::cin, rubbish);
            }
        }
        head = tail = nullptr;
        for (int i = 0; i < this->clause_count; i++) {
            int var;
            while (true) {
                std::cin >> var;
                struct ELEM* elem = new ELEM();
                elem->val = var;
                addElemToInfo(elem);
                if (var == 0) break;
            }
        }
    }

    void gatherInfo() {
        struct ELEM* elem = head;
        int clause_idx = 1, count = 0;
        while (elem) {
            if (elem->val == 0) {
                clause_capacity[clause_idx] = count;
                count = 0;
                clause_idx++;
            }
            else {
                const int idx = elem->val > 0 ? elem->val : bool_var_count - elem->val;
                number_capacity[idx]++;
                count ++;
            }
            elem = elem->next;
        }
    }

    void print() {
        struct ELEM* elem = head;
        while (elem) {
            if (elem->val == 0) std::cout << std::endl;
            else std::cout << elem->val << " ";
            elem = elem->next;
        }
    }

    void checkAnswer() {
        struct ELEM* elem = head;
        bool flag = false;
        while (elem) {
            if (elem->val == 0) {
                if (flag == false) {
                    std::cout << "Wrong Answer\n";
                    return;
                }
                flag = false;
            }
            else {
                if (elem->val == abs(elem->val) * assign[abs(elem->val)]) {
                    flag = true;
                }
            }
            elem = elem->next;
        }
    }

    void init() {
        gatherInfo();
        number_count = 2 * bool_var_count;
        empty_clause = empty_root = false;
        unit.init();
        modify.init();
        memset(assign, 0, sizeof(assign));
        for (int i = 1; i <= clause_count; i++) {
            clause[i].init(clause_capacity[i]);
            exist[i] = true;
            invalid[i] = 0;
        }
        for (int i = 1; i <= number_count; i++) {
            number[i].init(number_capacity[i]);
        }

        struct ELEM* elem = head;
        int clause_idx = 1, clause_slot = 0, number_slot[number_count + 1];
        memset(number_slot, 0, sizeof(number_slot)); 
        while (elem) {
            if (elem->val == 0) {
                clause_idx ++;
                clause_slot = 0;
            }
            else {
                const int number_idx = elem->val > 0 ? elem->val : bool_var_count - elem->val;
                number[number_idx].set(number_slot[number_idx], clause_idx, clause_slot);
                clause[clause_idx].set(clause_slot, number_idx, number_slot[number_idx]);
                number_slot[number_idx] ++;
                clause_slot ++;
            }
            elem = elem->next;
        }
    }

    int negatedVar(const int var) const {
        return var > bool_var_count ? var - bool_var_count : var + bool_var_count;
    }

    bool setClauseInvalid(const int clause_idx) {
        invalid[clause_idx] ++;
        if (exist[clause_idx]) {
            exist[clause_idx] = false;
            return true;
        }
        return false;
    }

    bool setClauseValid(const int clause_idx) {
        invalid[clause_idx] --;
        if (invalid[clause_idx] == 0)
            exist[clause_idx] = true;
        if (exist[clause_idx]) return true;
        else return false;
    }

    void removeClauseByVar(const int var) {
        for (int i = 0; i < number[var].capacity; i++) {
            const int clause_idx = number[var].idx[i];
            if (setClauseInvalid(clause_idx))
                clause_count --;
        }
        if (clause_count == 0) empty_root = true;
    }

    void restoreClauseByVar(const int var) {
        for (int i = 0; i < number[var].capacity; i++) {
            const int clause_idx = number[var].idx[i];
            if (setClauseValid(clause_idx))
                clause_count ++;
        }
    }

    void removeLiteralFromClause(const int var) {
        for (int i = 0; i < number[var].capacity; i++) {
            const int clause_idx = number[var].idx[i];
            const int clause_slot = number[var].slot[i];
            if (exist[clause_idx]) {
                clause[clause_idx].remove(clause_slot);
                PAIR pair = clause[clause_idx].get(clause_slot);
                number[pair.first].set(pair.second, clause_idx, clause_slot);
                if (clause[clause_idx].capacity == 1) {
                    unit.push(clause_idx);
                }
                else if (clause[clause_idx].capacity == 0) {
                    empty_clause = true;
                }
            }
        }
    }

    void restoreLiteralToClause(const int var) {
        for (int i = 0; i < number[var].capacity; i++) {
            const int clause_idx = number[var].idx[i];
            if (exist[clause_idx]) {
                number[var].set(i, clause_idx, clause[clause_idx].capacity);
                clause[clause_idx].add(var, i);
            }
        }
    }

    void assignVar(const int var) {
        modify.push(var);
        if (var > bool_var_count) assign[var - bool_var_count] = -1;
        else assign[var] = 1;
    }

    void reassignVar(const int var) {
        const int idx = var > bool_var_count ? var - bool_var_count : var;
        assign[idx] = 0;
    }

    void restoreToTop(const int top) {
        empty_clause = false;
        unit.clear();
        while (modify.top > top) {
            const int var = modify.pop();
            reassignVar(var);
            restoreClauseByVar(var);
            restoreLiteralToClause(negatedVar(var));
        }
    }

    int chooseVar() const {
        for (int i = 1; i <= bool_var_count; i++) {
            if (assign[i] == 0) return i;
        }
        return 0;
    }

    void actionForVar(const int var) {
        const int top = modify.top;
        assignVar(var);
        removeClauseByVar(var);
        removeLiteralFromClause(negatedVar(var));
        if (empty_clause == false) {
            while (!unit.empty()) {
                const int clause_idx = unit.pop();
                if (exist[clause_idx]) {
                    const int var = clause[clause_idx].idx[0];
                    assignVar(var);
                    removeClauseByVar(var);
                    removeLiteralFromClause(negatedVar(var));
                    if (empty_clause) break;
                }
            }
        }
        if (empty_root == false && empty_clause == false)
            solve();
        if (empty_root == true) return;
        else restoreToTop(top);
    }

    void solve() {
        const int var = chooseVar();
        if (var == 0) return;
        std::cout << "Choose " << var << std::endl;
        actionForVar(var);
        if (empty_root) return;
        actionForVar(negatedVar(var));
    }

    void printAnswer() {
        for (int i = 1; i <= bool_var_count; i++) {
            if (assign[i] == 0) std::cout << i << std::endl;
            else std::cout << assign[i] * i << std::endl;
        }
        if (empty_root) std::cout << "SAT\n";
        else std::cout << "UNSAT\n";
    }
};

#endif