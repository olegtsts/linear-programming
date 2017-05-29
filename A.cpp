#include "glpk.h"
#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;
    std::vector<double> c(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> c[i * n + j];
        }
    }
    glp_prob *lp;
    int ia[125001], ja[125001];
    double ar[125001];
    double z;
    double x[2501];
    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_OFF;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "sample");
    glp_set_obj_dir(lp, GLP_MAX);
    glp_add_rows(lp, n);
    for (int i = 0; i < n; ++i) {
        glp_set_row_bnds(lp, i + 1, GLP_FX, 1, 1);
    }
    glp_add_cols(lp, n * n);
    for (int i = 0; i < n * n; ++i) {
        glp_set_col_bnds(lp, i + 1, GLP_DB, 0, 1);
        glp_set_obj_coef(lp, i + 1, -c[i]);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n * n; ++j) {
            int global_index = i * n * n + j;
            ia[global_index + 1] = i + 1;
            ja[global_index + 1] = j + 1;
            ar[global_index + 1] = (j / n) == i ? 1 : 0;
        }
    }
    glp_load_matrix(lp, n * n * n, ia, ja, ar);
    glp_simplex(lp, &params);
    z = -glp_get_obj_val(lp);

    std::cout << int(z + 0.5) << std::endl;

    return 0;
}
