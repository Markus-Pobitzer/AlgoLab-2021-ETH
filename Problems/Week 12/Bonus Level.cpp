#include <iostream>
#include <vector>


// Going down and back up again is equivalent to going down twice since this is the definition of the possible movement.
// Store the runs in a memo table if we think of goining two steps at once
//
// In each step we will mov one diagonal
//
// We keep a dynamic programming state:
//
// memo[][][]
//
// The first dimension represents the diagonal of the step: x + y.
// The second dimension represents the x-position of the first travel.
// The third dimension represents the x-position of the second travel.
//
// The value in the array is the total number of coins that can be collected when starting from that point.
// Solution at memo[0][0][0]

int recursive(int d, int x1, int x2, const int n, const std::vector<std::vector<int>>& game, std::vector<std::vector<std::vector<int>>>& memo) {
    if (d >= 2 * n - 1 || x1 >= n || x2 >= n || d - x1 >= n || d - x2 >= n) {
        return 0;
    }

    if (memo[d][x1][x2] >= 0) {
        return memo[d][x1][x2];
    }
    int ret = game[d - x1][x1];
    if (x1 != x2) {
        ret += game[d - x2][x2];
    }

    ret += std::max(std::max(std::max(recursive(d + 1, x1 + 1, x2 + 1, n, game, memo), recursive(d + 1, x1 + 1, x2, n, game, memo)), recursive(d + 1, x1, x2 + 1, n, game, memo)), recursive(d + 1, x1, x2, n, game, memo));
    memo[d][x1][x2] = ret;
    return ret;
}

void testCase() {
    int n; std::cin >> n;

    std::vector<std::vector<int>> game(n, std::vector<int>(n));
    std::vector<std::vector<std::vector<int>>> memo(2 * n - 1, std::vector<std::vector<int>>(n, std::vector<int>(n, -1)));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> game[i][j];
        }
    }

    std::cout << recursive(0, 0, 0, n, game, memo) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;

    while (t--) {
        testCase();
    }
}