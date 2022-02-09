#include <iostream>
#include <vector>
#include <cmath>

/*
  Solved with DP and intelligent indexing. Note that we need to set strict upper
  bouds for our DP/Memo table otherwise it gets to big/slow. Our Memo table has
  dimesnions n x 16 x 16 x 12. The two 16 arrive from the indexing of the possbile
  last two fighter combinations (see getIndex()) where the biggest index is
  4 * 3 + 3 = 15. The last dimension 12 is fixed sinc it represents the ¦p - q¦,
  i.e. the abs difference of the total umbe of fighters i the two gates.

  The current state gets represented as a position (pos) of the current fighter,
  if we want to send it north, the last m fighters sent from the north/south
  entrance, encoded as an integer 1abc where c is the last fighter send to this
  gate, and tthe integers p & q representing the total number of fighters sent
  to the north / south gate.

  We map a state to a memo entry as follows:
  north_i = 1abc
  south_i = 1def

  if(p > q)
    memo[pos][4 * b + c][4 * e + f][¦p - q¦]
  else
    memo[pos][4 * e + f][4 * b + c][¦p - q¦]
*/

/*
  Counts how many different fighters in index 1abc are. a, b, c in {0, 1, 2, 3}
*/
int distinctive(int index, int stop) {
    int ret = 0;
    std::vector<bool> visited(4, false);
    for (int i = 0; i < stop; i++) {
        int p = index % 10;
        index = index / 10;
        if (!visited[p]) {
            visited[p] = true;
            ret++;
        }
    }
    return ret;
}

/*
  From the in index 1abc we get an index representation as follows:
  ret = 4 * b + c;
*/

int getIndex(int in, int m) {
    int ret = 0;
    for (int i = 0; i < m - 1; i++) {
        ret += std::pow(4, i) * (in % 10);
        in /= 10;
    }
    return ret;
}

int recursive(int pos, const std::vector<int>& fighters, bool north, int north_i, int south_i, int p, int q, const int m, std::vector<std::vector<std::vector<std::vector<int>>>>& memo) {
    if (pos >= (int)fighters.size())
        return 0;
    int distinct = 0;
    if (north) {
        north_i = (north_i % (int)std::pow(10, m - 1)) * 10 + fighters[pos] + 1000;
        p++;
        distinct = distinctive(north_i, std::min(m, p));
    }
    else {
        south_i = (south_i % (int)std::pow(10, m - 1)) * 10 + fighters[pos] + 1000;
        q++;
        distinct = distinctive(south_i, std::min(m, q));
    }

    int abpq = std::abs(p - q);
    int ret = distinct * 1000 - std::pow(2, abpq);

    if (ret >= 0) {
        int ni = (p > q) ? getIndex(north_i, m) : getIndex(south_i, m);
        int si = (p > q) ? getIndex(south_i, m) : getIndex(north_i, m);

        if (memo[pos][ni][si][abpq] != -1) {
            return ret + memo[pos][ni][si][abpq];
        }

        int max = recursive(pos + 1, fighters, true, north_i, south_i, p, q, m, memo);
        max = std::max(max, recursive(pos + 1, fighters, false, north_i, south_i, p, q, m, memo));
        ret += max;
        memo[pos][ni][si][abpq] = max;
    }
    return ret;
}

void testCase() {
    int n, k, m;
    std::cin >> n >> k >> m;
    std::vector<int> fighters(n);
    for (int i = 0; i < n; i++) {
        std::cin >> fighters[i];
    }

    std::vector<std::vector<std::vector<std::vector<int>>>> memo(n, std::vector<std::vector<std::vector<int>>>(16, std::vector<std::vector<int>>(16, std::vector<int>(12, -1))));
    std::cout << recursive(0, fighters, false, 1000, 1000, 0, 0, m, memo) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
}