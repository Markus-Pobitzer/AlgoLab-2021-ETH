#include <iostream>
#include <vector>
#include <algorithm>

/*
  * Sort the B potions by strength. For the A potions create a 3D memo table with
  * (n+1) x (n+1) x (H+1) dimensions. i.e. for every potions with at most n potion
  * set the maximum power level for h happines value. Solution in [0][j][H].
*/
void testCase(int t) {
    bool debug = false;
    if (t == 0) {
        debug = false;
    }
    int n, m, a, b, P, H, W, ret;
    std::cin >> n >> m >> a >> b >> P >> H >> W;
    std::vector<std::pair<long, long>> A(n);
    std::vector<long> B(m);
    ret = -1;

    if (debug) {
        std::cout << n << " " << m << "\n" << a << " " << b << "\n" << P << " " << H << " " << W << "\n";
    }

    for (int i = 0; i < n; i++) {
        long pi, hi;
        std::cin >> pi >> hi;
        A[i] = std::make_pair(pi, hi);
    }

    for (int i = 0; i < m; i++) {
        int wi;
        std::cin >> wi;
        B[i] = wi;
    }

    // std::sort(A.begin(), A.end());

    std::sort(B.begin(), B.end());

    int cM = 0;

    long long neededP = P; // The Power we still need
    long long neededW = 0; // The Wits we still need

    for (int i = m - 1; neededW < W && i >= 0; i--) {
        neededW += B[i];
        if (debug) {
            std::cout << i << ": " << B[i] << " and wits: " << neededW << "\n";
        }
        cM++;
    }

    if (neededW < W) {
        std::cout << -1 << "\n";
        return;
    }
    neededP += ((long)cM) * ((long)b);
    if (debug)
        std::cout << "Power: " << neededP << "\n";

    std::vector<std::vector<std::vector<long long>>> memo(n + 1, std::vector<std::vector<long long>>(n + 1, std::vector<long long>(H + 1, 0)));
    for (int i = n - 1; i >= 0; i--) {
        for (int j = 1; j <= n; j++) {
            for (int h = 0; h <= H; h++) {
                long long cur = 0;
                if (A[i].second >= h) {
                    cur = A[i].first + memo[i + 1][j - 1][0];
                }
                else if (memo[i + 1][j - 1][h - A[i].second] > 0) {
                    cur = A[i].first + memo[i + 1][j - 1][h - A[i].second];
                }
                memo[i][j][h] = std::max(cur, memo[i + 1][j][h]);
            }
        }
    }

    for (int i = 0; ret < 0 && i <= n && cM <= m; i++) {
        if (memo[0][i][H] >= neededP) {
            if (neededW - ((long)a) * ((long)i) >= (long)W) {
                ret = i + cM;
                if (debug) {
                    std::cout << "i: " << i << " B: " << cM << "\n";
                }
            }
            else {
                while (ret < 0 && i < n && cM < m) {
                    while (cM < m && neededW - ((long)a) * ((long)i) < (long)W) {
                        neededW += B[m - cM - 1];
                        cM++;
                    }
                    while (i < n && memo[0][i][H] < (long)P + ((long)cM) * ((long)b)) {
                        i++;
                    }
                    if (memo[0][i][H] >= (long)P + ((long)cM) * ((long)b) && neededW - ((long)a) * ((long)i) >= W) {
                        ret = i + cM;
                    }
                }
            }
        }
    }

    if (debug) {
        std::cout << "Table: \n";
        for (int i = 0; i <= n; i++) {
            std::cout << i << ": " << memo[0][i][H] << "\n";
        }
    }
    std::cout << ret << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
}