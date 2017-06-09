#include <iostream>
#include <fstream>
#include <vector>

void generate_test() {
    std::ofstream out("input.txt");

    int n = rand() % 25 + 2;
    int m = rand() % 25 + 1;
    if (m >= n * (n - 1) / 2) {
        m = n * (n - 1) / 2;
    }
    out << n << " " << m << std::endl;
    std::vector<bool> met_pair(n * n, false);
    for (int i = 0; i < m; ++i) {
        int first_index;
        int second_index;
        do {
            first_index = rand() % n;
            second_index = rand() % n;
        } while (met_pair[first_index * n + second_index] || first_index == second_index);
        met_pair[first_index * n + second_index] = true;
        out << first_index + 1 << " " << second_index + 1 << " " << rand() % 5 << std::endl;
    }
    std::cout << "Test is generated" << std::endl;
}

void launch_programms() {
    if (system("./flow < input.txt > output1 && ./C.out < input.txt > output2 && diff output1 output2")) {
        throw 1;
    } else {
        std::cout << "OK" << std::endl;
    }
}

int main() {
    for (size_t i = 0; i < 1000; ++i) {
        generate_test();
        launch_programms();
    }
    return 0;
}
