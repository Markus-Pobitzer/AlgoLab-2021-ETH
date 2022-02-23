#include <iostream>
#include <vector>

void testCase() {
    int n, m, k;
    long x;
    std::cin >> n >> m >> x >> k;

    std::vector<bool> end(n, true);
    std::vector<std::vector<int>> g(n, std::vector<int>(n, -1));

    for (int i = 0; i < m; i++) {
        int u, v, p;
        std::cin >> u >> v >> p;
        g[u][v] = std::max(p, g[u][v]);
        end[u] = false;
    }

    std::vector<std::vector<std::pair<int, int>>> G(n);
    std::vector<int> End;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] != -1) {
                G[i].push_back(std::make_pair(j, g[i][j]));
            }
        }
        if (end[i]) {
            End.push_back(i);
        }
    }

    std::vector<long> first(n, -1);
    std::vector<long> second(n, -1);
    second[0] = 0;
    first[0] = 0;
    int ret = -1;
    for (int i = 1; ret == -1 && i <= k; i++) {
        for (int j = 0; j < n; j++) {
            for (std::pair<int, int> p : G[j]) {
                if (second[j] != -1) {
                    first[p.first] = std::max(first[p.first], second[j] + p.second);
                    if (first[p.first] >= x) {
                        ret = i;
                    }
                }
            }
        }

        for (int j : End) {
            if (first[j] != -1) {
                first[0] = std::max(std::max(second[0], first[0]), first[j]);
            }
        }

        std::vector<long> swap = first;
        first = second;
        second = swap;
    }

    if (ret == -1)
        std::cout << "Impossible\n";
    else
        std::cout << ret << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
    return 0;
}