#include <iostream>

int testcase() {
    int ret = 0;
    int i; std::cin >> i;
    int k;
    for (int j = 0; j < i; j++) {
        std::cin >> k;
        ret += k;
    }

    return ret;
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        std::cout << testcase() << std::endl;
    }
}