///3
#include <vector>
#include <iostream>
#include <algorithm>

int recOne(int index, const std::vector<std::vector<std::vector<int>>>& graph, std::vector<std::vector<std::vector<int>>>& path, std::vector<int>& depth) {
    int ret = 0;
    for (int i = 0; i < (int)graph[index].size(); i++) {
        //std::cout << index << " calls " << graph[index][i][1] << std::endl;
        int ci = graph[index][i][1] + recOne(graph[index][i][0], graph, path, depth); // The ccost to vsit the child node
        int di = depth[graph[index][i][0]] + 1; // The depth of the child node plus th child node itsel
        std::vector<int> local = { ci, graph[index][i][0], graph[index][i][1], di };
        path[index].push_back(local);
        ret += ci;
        depth[index] += di;
    }
    std::sort(path[index].begin(), path[index].end(), [](auto e1, auto e2) -> bool {return ((long)e1[0]) * e2[3] < ((long)e2[0]) * e1[3]; });
    // std::cout << index << ": " << path[index].size() << std::endl;
    return ret;
}

long recTwo(int current, std::vector<long>& value, std::vector<std::vector<std::vector<int>>>& path, long& dim) {
    long ret = value[current] - dim;
    //std::cout << current << ": " << ret << " dim: " << dim << std::endl;
    for (int i = 0; i < (int)path[current].size(); i++) {
        dim += path[current][i][2];
        ret += recTwo(path[current][i][1], value, path, dim);
        dim += path[current][i][2];
    }
    return ret;
}

void testCase() {
    int n;
    std::cin >> n;
    std::vector<long> value(n + 1);
    for (int i = 1; i <= n; i++) {
        std::cin >> value[i];
    }

    std::vector<std::vector<std::vector<int>>> graph(n + 1);
    std::vector<std::vector<std::vector<int>>> path(n + 1);
    std::vector<int> depth(n + 1);
    for (int i = 0; i < n; i++) {
        int u, v, l;
        std::cin >> u >> v >> l;
        std::vector<int> vl = { v, l };
        graph[u].push_back(vl);
    }

    recOne(0, graph, path, depth);
    long dim = 0;
    std::cout << recTwo(0, value, path, dim) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
}