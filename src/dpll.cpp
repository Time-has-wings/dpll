#include "dpll.h"

static void basicSolve(ROOT* root, int* ans) {
    int backup_ans[root->bool_var_count + 1];
    for (int i = 1; i <= root->bool_var_count; i++) {
        backup_ans[i] = ans[i];
    }
    while (true) {
        CLAUSE* unit_clause = findOneUnitClause(root);
        if (unit_clause == nullptr) break;
        else {
            SIMPLIFYRESULT result = simplifyRoot(root, unit_clause->head_literal->bool_var);
            ans[abs(unit_clause->head_literal->bool_var)] = unit_clause->head_literal->negated ? -1 : 1;
            if (result == EMPTY_CLAUSE) return;
            else if (result == EMPTY_ROOT) {
                root->solved = true;
                return;
            }
            else;
        }
    }
    int bool_val = -1;
    for (int i = 1; i <= root->bool_var_count; i++) {
        if (ans[i] == 0) {
            bool_val = i;
            break;
        }
    }
    if (bool_val == -1) {
        for (int i = 1; i <= root->bool_var_count; i++) {
            ans[i] = backup_ans[i];
        }
        return;
    }
    ROOT* backup_root = backupRoot(root);
    ans[bool_val] = 1;
    createOneSingleClause(root, bool_val);
    basicSolve(root, ans);
    if (root->solved) {
        releaseRoot(backup_root);
        return;
    }
    ans[bool_val] = -1;
    createOneSingleClause(backup_root, -bool_val);
    basicSolve(backup_root, ans);
    root->solved = backup_root->solved;
    if (backup_root->solved) {
        releaseRoot(backup_root);
        return;
    }
    releaseRoot(backup_root);
    for (int i = 1; i <= root->bool_var_count; i++) {
        ans[i] = backup_ans[i];
    }
}

void basicDpll(ROOT* root) {
    ROOT* backup_root = backupRoot(root);
    int* ans = new int[backup_root->bool_var_count + 1];
    memset(ans, 0, sizeof(int)*(backup_root->bool_var_count + 1));
    clock_t start = clock();
    basicSolve(backup_root, ans);
    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC * 1000;
    if (backup_root->solved) {
        checkAnswer(root, ans);
        std::cout << "SAT\n";
        for (int i = 1; i <= backup_root->bool_var_count; i++) {
            if (ans[i] == 0) std::cout << i << std::endl;
            else std::cout << i * ans[i] << std::endl;
        }
    }
    else std::cout << "UNSAT\n";
    std::cout << "Time: " << duration << "ms\n";
    delete ans;
    delete backup_root;
}