#include <iostream>
#include <vector>

/*
  Min maxing of two playes. We have a Memo table with 2 values for each position
  in the Game. The first tells us the min numbe rof steps to reach the end the
  second one the max steps. So for a new game position further down we get th min
  value as the minimum over all max values of the reachable fields +1 and for the
  max value we get the maximum over all min values of the reachable fields.

  In the end we compare the min value of the start positions. If one is smaller
  we know who won otherwise we check who was on the turn. Every altenting turn we
  are either wining or losing: hops % 4 == 1 and red is winning.

  Pay attention to the index starts at 1 and the result is 0 if red wins!

*/
void testCase() {
    int n, m, r, b;
    std::cin >> n >> m >> r >> b;

    std::vector<std::vector<int>> con(n);
    for (int i = 0; i < n; i++) {
        con[i] = std::vector<int>();
    }

    for (int i = 0; i < m; i++) {
        int u, v; std::cin >> u >> v;
        con[u - 1].push_back(v - 1);
    }

    std::vector<std::vector<int>> memo(n, std::vector<int>(2, -1));
    memo[n - 1][0] = 0; // For the min
    memo[n - 1][1] = 0; // For the max

    for (int j = n - 2; j >= std::min(r - 1, b - 1); j--) {
        int min = n;
        int max = 0;
        for (int i = 0; i < (int)con[j].size(); i++) {
            int v = con[j][i];
            min = std::min(min, memo[v][1]); // The new min is the minimum over all reachable max values
            max = std::max(max, memo[v][0]); // The new max is the maximum over all reachable min values
        }
        memo[j][0] = min + 1;
        memo[j][1] = max + 1;
    }


    int red = memo[r - 1][0];
    int black = memo[b - 1][0];
    int ret = 1;
    if (red < black || (red == black && red % 2 == 1)) {
        ret = 0;
    }
    /*
    if (red <= 1 || (red)/2 < (black+1)/2) {
      ret = 0;
    } else {
      ret = 1;
    }
    */
    std::cout << ret << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
}