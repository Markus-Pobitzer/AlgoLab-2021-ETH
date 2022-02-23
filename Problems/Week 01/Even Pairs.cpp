#include <iostream>
#include <vector>

int testCase() {
    int ret = 0;
    int n; std::cin >> n;
    std::vector<int> psum(n, 0);
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            std::cin >> psum[0];
        }
        else {
            std::cin >> psum[i];
            psum[i] += psum[i - 1];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            int ls = 0;
            if (j == 0) {
                ls = psum[i];
            }
            else {
                ls = psum[i] - psum[j - 1];
            }
            if (ls % 2 == 0) {
                ret++;
            }
        }
    }

    return ret;
}

int testCaseImproved() {
    int ret = 0;
    int n; std::cin >> n;
    std::vector<int> psum(n, 0);
    for (int i = 0; i < n; i++) {
        if (i == 0) {
            std::cin >> psum[i];
        }
        else {
            std::cin >> psum[i];
            psum[i] += psum[i - 1];
        }
        if (psum[i] % 2 == 0) {
            ret++;
        }
    }

    int e = ret * (ret - 1) / 2;
    int o = (n - ret) * (n - ret - 1) / 2;
    ret += e + o;

    return ret;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        std::cout << testCaseImproved() << std::endl;
    }
}