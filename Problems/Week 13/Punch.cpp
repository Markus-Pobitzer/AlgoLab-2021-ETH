///2
#include <iostream>
#include <vector>

/*
* Solved with DP table. Each row corresponds to a beverage. Go from left to right and from first row to last one.
* Store the min cost and max number of different beverages in each entry (min cost is more important).
* If you just take an exisiting value do just copy other wise add 1 to the amount of different bevarages.
*
*/
void testCase(int t) {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> c(n);
    std::vector<int> v(n);

    for (int i = 0; i < n; i++) {
        std::cin >> c[i] >> v[i];
    }

    std::vector<std::vector<int>> dpC(n, std::vector<int>(k + 1, 999999999));
    std::vector<std::vector<int>> dpP(n, std::vector<int>(k + 1, 0));

    dpC[0][0] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < k + v[i]; j++) {
            int index = std::min(j, k);
            int cost = dpC[i][index];
            int punch = dpP[i][index];

            if (v[i] >= j && c[i] < cost) {
                cost = c[i];
                punch = 1;
            }


            if (v[i] < j) {
                if (dpC[i][j - v[i]] + c[i] < cost || (dpC[i][j - v[i]] + c[i] == cost && dpP[i][j - v[i]] > punch)) {
                    cost = dpC[i][j - v[i]] + c[i];
                    punch = dpP[i][j - v[i]];
                }
            }

            if (i != 0) {
                if (dpC[i - 1][index] < cost || (dpC[i - 1][index] == cost && dpP[i - 1][index] > punch)) {
                    cost = dpC[i - 1][index];
                    punch = dpP[i - 1][index];
                }

                if (j >= v[i]) {
                    if (dpC[i - 1][j - v[i]] + c[i] < cost || (dpC[i - 1][j - v[i]] + c[i] == cost && dpP[i - 1][j - v[i]] + 1 > punch)) {
                        cost = dpC[i - 1][j - v[i]] + c[i];
                        punch = dpP[i - 1][j - v[i]] + 1;
                    }
                }
            }

            dpC[i][index] = cost;
            dpP[i][index] = punch;

            if (index <= k || index == k + v[i]) {
                // std::cout << " (" << cost << ", " << punch << ")";
            }
        }
        // std::cout << "\n";
    }

    std::cout << dpC[n - 1][k] << " " << dpP[n - 1][k] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
    return 0;
}