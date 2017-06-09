#include <iostream>
#include <vector>

bool dfs(
        std::vector<bool>& is_visited,
        const int start_point,
        const int m,
        const std::vector<bool> edge,
        std::vector<std::pair<int, int> >& increasing_path,
        std::vector<int> &right_to_left_couple
) {
    if (!is_visited[start_point]) {
        is_visited[start_point] = true;
        for (int i = 0; i < m; ++i) {
            if (edge[start_point * m + i]) {
                int right_vertex = i;
                increasing_path.push_back(std::make_pair(start_point, right_vertex));
                int new_left_vertex = right_to_left_couple[right_vertex];
                if (new_left_vertex >= 0) {
                    if (dfs(is_visited, new_left_vertex, m, edge, increasing_path, right_to_left_couple)) {
                        return true;
                    }
                } else {
                    return true;
                }
                increasing_path.pop_back();
            }
        }
    }
    return false;
}

void final_dfs(
        std::vector<int>& right_to_left_couple,
        std::vector<bool>& left_visited,
        std::vector<bool>& right_visited,
        const int vertex,
        const bool is_left,
        const int n,
        const int m,
        const std::vector<bool> edge
) {
    if (is_left) {
        if (!left_visited[vertex]) {
            left_visited[vertex] = true;
            for (size_t j = 0; j < m; ++j) {
                if (edge[vertex * m + j] && right_to_left_couple[j] != vertex) {
                    final_dfs(right_to_left_couple, left_visited, right_visited, j, false, n, m, edge);
                }
            }
        }
    } else {
        if (!right_visited[vertex]) {
            right_visited[vertex] = true;
            for (size_t i = 0; i < n; ++i) {
                if (edge[i * m + vertex] && right_to_left_couple[vertex] == i) {
                    final_dfs(right_to_left_couple, left_visited, right_visited, i, true, n, m, edge);
                }
            }
        }
    }
}


int main() {
    long long int n,m,k;
    std::cin >> n >> m >> k;
    std::vector<bool> edge(n * m, false);
    for (int i = 0; i < k; ++i) {
        int a,b;
        std::cin >> a >> b;
        a -= 1;
        b -= 1;

        edge[a * m + b] = true;
    }

    std::vector<int> right_to_left_couple(m, -1);
    for (size_t i = 0; i < n; ++i) {
        std::vector<bool> is_visited(n, false);
        std::vector<std::pair<int, int> > increasing_path;
        dfs(is_visited, i, m, edge, increasing_path, right_to_left_couple);
        for (auto& pair: increasing_path) {
            int left = pair.first;
            int right = pair.second;
            right_to_left_couple[right] = left;
        }
    }

    std::vector<bool> left_start_traverse(n, true);
    for (auto& left: right_to_left_couple) {
        if (left >= 0) {
            left_start_traverse[left] = false;
        }
    }

    std::vector<bool> left_visited(n, false);
    std::vector<bool> right_visited(m, false);
    for (size_t i = 0; i < n; ++i) {
        if (left_start_traverse[i]) {
            final_dfs(right_to_left_couple, left_visited, right_visited, i, true, n, m, edge);
        }
    }

    std::vector<int> left_nodes;
    std::vector<int> right_nodes;
    for (size_t i = 0; i < n; ++i) {
        if (!left_visited[i]) {
            left_nodes.push_back(i);
        }
    }
    for (size_t j = 0; j < m; ++j) {
        if (right_visited[j]) {
            right_nodes.push_back(j);
        }
    }

    std::cout << left_nodes.size() + right_nodes.size() << std::endl;
    std::cout << left_nodes.size();
    for (auto& node: left_nodes) {
        std::cout << " " << node + 1;
    }
    std::cout << std::endl;
    std::cout << right_nodes.size();
    for (auto& node: right_nodes) {
        std::cout << " " << node + 1;
    }
    std::cout << std::endl;

    return 0;
}
