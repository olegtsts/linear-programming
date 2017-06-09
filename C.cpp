#include <iostream>
#include <vector>
#include "glpk.h"

int main () {
    int n,m;
    std::cin >> n >> m;
    std::vector<int> first_indices(m);
    std::vector<int> second_indices(m);
    std::vector<double> max_flow(m);

    for (size_t i = 0; i < m; ++i) {
        int a,b;
        double c;
        std::cin >> a >> b >> c;
        first_indices[i] = a - 1;
        second_indices[i] = b - 1;
        max_flow[i] = c;
    }

    glp_prob *lp;
    int ia[10001];
    int ja[10001];
    double ar[10001];
    lp = glp_create_prob();
    glp_set_obj_dir(lp, GLP_MAX);
    glp_add_rows(lp, n - 1);
    glp_set_row_bnds(lp, 1, GLP_LO, 0, 0);
    for (size_t i = 1; i + 1 < n; ++i) {
        glp_set_row_bnds(lp, i + 1, GLP_FX, 0, 0);
    }
    glp_add_cols(lp, m);
    for (size_t j = 0; j < m; ++j) {
        if (max_flow[j] > 0) {
            glp_set_col_bnds(lp, j + 1, GLP_DB, 0, max_flow[j]);
        } else {
            glp_set_col_bnds(lp, j + 1, GLP_FX, 0, 0);
        }
        int first_index = first_indices[j];
        int second_index = second_indices[j];
        if (first_index == 0) {
            glp_set_obj_coef(lp, j + 1, 1);
        } else if (second_index == 0) {
            glp_set_obj_coef(lp, j + 1, -1);
        } else {
            glp_set_obj_coef(lp, j + 1, 0);
        }
    }
    for (size_t i = 0; i + 1 < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            int global_index = i * m + j;
            ia[global_index + 1] = i + 1;
            ja[global_index + 1] = j + 1;
            int first_index = first_indices[j];
            int second_index = second_indices[j];
            if (first_index == i) {
                ar[global_index + 1] = 1;
            } else if (second_index == i) {
                ar[global_index + 1] = -1;
            } else {
                ar[global_index + 1] = 0;
            }
        }
    }
    glp_load_matrix(lp, (n - 1) * m, ia, ja, ar);
    glp_smcp params;
    glp_init_smcp(&params);
    params.msg_lev = GLP_MSG_OFF;
    glp_simplex(lp, &params);
    std::cout << int(glp_get_obj_val(lp)) << std::endl;
    for (size_t j = 0; j < m; ++j) {
        std::cout << int(glp_get_col_prim(lp, j + 1));
        if (j + 1 < m) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
