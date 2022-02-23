#include <iostream>
#include <vector>

/*
  Create a 2D table with prefix sum: ps[i][j] = ps[i - 1][j] + ps[i][j - 1] +
  - ps[i - 1][j - 1] + value; than based on this look at every 2D line (i.e.
  several rows starting at c and ending at d with #a elements peer row). Based
  on the prefix sum compute how many are odd / even. And aftr we went over all a
  for this c, d combination add to the result:
  ret += (even * (even - 1))/2 + even + (odd * (odd - 1)) / 2
*/

void testCase() {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> ps(n + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int val; std::cin >> val;
            ps[i][j] = ps[i - 1][j] + ps[i][j - 1] - ps[i - 1][j - 1] + val;
            //ps[i][j] = ps[i-1][j] + ps[i][j-1] - ps[i-1][j-1] + val;
        }
    }

    int sum = 0;
    for (int c = 1; c <= n; c++) {
        for (int d = c; d <= n; d++) {
            int even = 0;
            int odd = 0;
            for (int a = 1; a <= n; a++) {
                if ((ps[d][a] - ps[c - 1][a]) % 2 == 0) {
                    even++;
                }
                else {
                    odd++;
                }
            }
            sum += (even * (even - 1)) / 2 + even + (odd * (odd - 1)) / 2;
        }
    }
    std::cout << sum << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
    return 0;
}