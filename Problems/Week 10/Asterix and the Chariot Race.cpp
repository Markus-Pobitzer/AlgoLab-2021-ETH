#include <iostream>
#include <vector>

/*
  Memo solution. See tha the whole strucutre is just a tree. So we start index 0
  with a DFS. For every node we have thre modes:
  0: Current node needs to be added
  1: parent go added (current node does not need to be added)
  2: parent did not get added but current node does not be added (but a child)

*/

int recursive(int index, int mode, const std::vector<std::vector<int>>& graph, const std::vector<int>& cost, std::vector<std::vector<int>>& memo) {
    if (memo[index][mode] >= 0) {
        return memo[index][mode];
    }
    else {
        int ret = 0;
        if (mode == 0) {
            ret += cost[index];
            for (int i = 0; i < (int)graph[index].size(); i++) {
                ret += recursive(graph[index][i], 1, graph, cost, memo);
            }
        }
        else if (mode == 1) {
            int c1 = cost[index];
            int c2 = 0;

            for (int i = 0; i < (int)graph[index].size(); i++) {
                // Every child has parent set to true
                c1 += recursive(graph[index][i], 1, graph, cost, memo);

                // Parent not set but child may be set
                c2 += recursive(graph[index][i], 2, graph, cost, memo);
            }

            ret = std::min(c1, c2);
        }
        else {
            if ((int)graph[index].size() == 0) {
                ret = cost[index];
            }
            else {
                int c1 = cost[index];
                int c2 = 0;

                int max = 0;
                std::vector<int> mode2((int)graph[index].size());
                std::vector<int> mode0((int)graph[index].size());
                for (int i = 0; i < (int)graph[index].size(); i++) {
                    // Every child has parent set to true
                    c1 += recursive(graph[index][i], 1, graph, cost, memo);

                    mode2[i] = recursive(graph[index][i], 2, graph, cost, memo);
                    max += mode2[i];
                    mode0[i] = recursive(graph[index][i], 0, graph, cost, memo);
                }

                for (int i = 0; i < (int)graph[index].size(); i++) {
                    if (i == 0) {
                        c2 = max - mode2[i] + mode0[i];
                    }
                    else {
                        c2 = std::min(c2, max - mode2[i] + mode0[i]);
                    }
                }

                ret = std::min(c1, c2);
            }
        }
        memo[index][mode] = ret;
        return ret;
    }
}

void testCase(int tc) {
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> graph(n);
    std::vector<int> cost(n);
    std::vector<std::vector<int>> memo(n, std::vector<int>(3, -1));
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
    }

    for (int i = 0; i < n; i++) {
        std::cin >> cost[i];
    }

    std::cout << recursive(0, 2, graph, cost, memo) << "\n";

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