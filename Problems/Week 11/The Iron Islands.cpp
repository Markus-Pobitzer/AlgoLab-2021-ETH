#include <iostream>
#include <vector>

/*
 Sliding window over one water path. And 1D Memo of legth k + 1 to store most islands
 that one is able to coquer when starting in the initial water path and include
 the middle node.

*/
void testCase(int ind) {
    int n, k, w;
    std::cin >> n >> k >> w;

    std::vector<std::vector<int>> ways(w);
    std::vector<int> cost(n);

    // std::cout << n << k << w;

    for (int i = 0; i < n; i++) {
        std::cin >> cost[i];
    }

    for (int i = 0; i < w; i++) {
        int l;
        std::cin >> l;
        ways[i] = std::vector<int>(l);
        for (int j = 0; j < l; j++) {
            std::cin >> ways[i][j];
        }
    }
    int max = 0;
    std::vector<int> memo(k + 1, 0);
    for (int i = w - 1; i >= 0; i--) {
        if (i != w - 1) {
            int j = 1;
            int lcost = 0;
            while (lcost <= k && j < (int)ways[i + 1].size()) {
                lcost += cost[ways[i + 1][j]];
                if (lcost <= k) {
                    if (i == w - 1)
                        memo[lcost] = j;
                    else
                        memo[lcost] = std::max(j, memo[lcost]);
                }
                j++;
            }
        }
        int cC = k;
        int start = 0;
        for (int j = 0; j < (int)ways[i].size(); j++) {
            cC -= cost[ways[i][j]];
            while (cC < 0 && start < j) {
                cC += cost[ways[i][start]];
                start++;
            }
            int lm = j - start + 1;
            if (cC > 0 && start == 0 && i != w - 1) {
                lm += memo[cC];
            }

            // std::cout << i << ": " << j << " " << cC << " " << lm << "\n";
            if ((cC == 0 || lm > (j - start + 1)) && lm > max) {
                max = lm;
            }
        }
    }


    std::cout << max << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
}