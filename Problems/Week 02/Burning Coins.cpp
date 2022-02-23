#include <iostream>
#include <vector>

int testCase() {
    int n; std::cin >> n;

    std::vector<int> val(n, 0);
    std::vector<int> tot(n, 0);
    for (int i = 0; i < n; i++) {
        std::cin >> val[i];
        tot[i] = val[i] + ((i == 0) ? 0 : tot[i - 1]);
    }

    std::vector<std::vector<int>> dp(n, std::vector<int>(n));

    for (int i = 0; i < n; i++) {
        dp[i][i] = val[i];
    }

    for (int j = 1; j < n; j++) {
        for (int i = 0; i < n - j; i++) {
            // pos[i, j + i]
            dp[i][j + i] = std::max(val[i] + tot[j + i] - tot[i] - dp[i + 1][j + i], val[j + i] + tot[j + i - 1] - ((i == 0) ? 0 : tot[i - 1]) - dp[i][j + i - 1]);
        }
    }

    /*
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        std::cout << dp[i][j] << " ";
      }
      std::cout << std::endl;
    }
    */

    return dp[0][n - 1];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;

    for (int i = 0; i < t; i++) {
        std::cout << testCase() << std::endl;
    }
}

int oldTestCase() {
    int ret = 0;
    int n; std::cin >> n;

    std::vector<int> val(n, 0);
    for (int i = 0; i < n; i++) {
        std::cin >> val[i];
    }
    int i = 0;
    int j = n - 1;
    for (int l = 0; l < n; l++) {
        //best bet
        int index;
        if (i == j) {
            index = i;
        }
        else if (j - i <= 2) {
            if (val[i] >= val[j]) {
                index = i;
                i++;
            }
            else {
                index = j;
                j--;
            }
        }
        else {
            if (val[i] - std::max(val[i + 1], val[j]) >= val[j] - std::max(val[i], val[j - 1])) {
                index = i;
                i++;
            }
            else {
                index = j;
                j--;
            }
        }

        if (l % 2 == 0) {
            ret += val[index];
        }
    }
    return ret;
}