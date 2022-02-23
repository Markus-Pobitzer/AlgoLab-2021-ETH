///1
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

/*
  Sort by start index of y0. Go from top to bottom ancd make checks with all rays
  that have positive slope and than another from bottom to top and make checks
  with rays of the negative slopes. Use exact costuctions for the slopes.
*/
typedef CGAL::Exact_predicates_exact_constructions_kernel K;

void testCase(int tc) {
    int n;
    std::cin >> n;
    std::vector<std::vector<K::FT>> inp(n, std::vector<K::FT>(3));

    for (int i = 0; i < n; i++) {
        long y0, x, y;
        std::cin >> y0 >> x >> y;
        inp[i][0] = K::FT(y0);
        inp[i][1] = (K::FT(y) - K::FT(y0)) / K::FT(x);
        inp[i][2] = i;
    }

    std::sort(inp.begin(), inp.end(), std::greater<std::vector<K::FT>>());

    K::FT minP(-1);
    K::FT maxN(1);
    std::vector<bool> isin(n, true);
    for (int i = 0; i < n; i++) {
        K::FT cs = inp[i][1];
        if (cs >= 0) {
            // For the positive parts
            if (minP == K::FT(-1)) {
                minP = cs;
            }
            else if (minP >= cs) {
                minP = cs;
            }
            else {
                isin[(int)CGAL::to_double(inp[i][2])] = false;
            }

            // For the negative parts
            if (maxN != 1 && maxN + cs > 0) {
                isin[(int)CGAL::to_double(inp[i][2])] = false;
            }
        }
        else if (maxN == 1 || maxN < cs) {
            maxN = cs;
        }
    }

    minP = K::FT(-1);
    maxN = K::FT(1);
    for (int i = n - 1; i >= 0; i--) {
        K::FT cs = inp[i][1];
        if (cs <= 0) {
            // For the negative parts
            if (maxN == K::FT(1)) {
                maxN = cs;
            }
            else if (maxN <= cs) {
                maxN = cs;
            }
            else {
                isin[(int)CGAL::to_double(inp[i][2])] = false;
            }

            // For the positive parts
            if (minP != -1 && minP + cs <= 0) {
                isin[(int)CGAL::to_double(inp[i][2])] = false;
            }
        }
        else if (minP == -1 || minP > cs) {
            minP = cs;
        }
    }

    for (int i = 0; i < n; i++) {
        if (isin[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase(t);
    }
    return 0;
}