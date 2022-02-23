#include <iostream>
#include <vector>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <queue>
#include <stack>

/*
  Comput MST, this is the base. Now we search for the biggest edge weight of each
  conection (u, v) in the MST. We use N DFS for this starting from eah vertex.
  Now we go over all the other edges not currently in the MST and search for the
  smallest difference of the edge (u, v) and tthe max edge on the path between u
  and v in the MST. The minum of all these values + the inital value of the MST is
  our return value.

  Recursive DFS was to slow :( Now with stack and queue.
*/

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property,
    boost::property<boost::edge_weight_t, int>
> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;


void dfs(int start, int current, int n, int max, const std::vector<std::vector<int>>& edges, const std::vector<std::vector<int>> mst, std::vector<bool> visited, std::vector<std::vector<int>>& ret) {
    ret[start][current] = max;
    ret[current][start] = max;
    visited[current] = true;
    for (int i = 0; i < (int)mst[current].size(); i++) { // Optimize mst
        if (!visited[mst[current][i]]) {
            dfs(start, mst[current][i], n, std::max(max, edges[current][mst[current][i]]), edges, mst, visited, ret);
        }
    }
}


void testCase() {
    int n, i;
    std::cin >> n >> i;

    weighted_graph G(n);
    std::vector<std::vector<int>> edges(n, std::vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int cj;
            std::cin >> cj;
            edges[i][j] = cj;
            edges[j][i] = cj;
            boost::add_edge(i, j, cj, G);
        }
    }

    int sum = 0;

    std::vector<std::vector<int>> mst_edges(n);
    std::vector<std::vector<bool>> isin(n, std::vector<bool>(n, false));
    std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        int from, to;
        from = boost::source(*it, G);
        to = boost::target(*it, G);
        mst_edges[from].push_back(to);
        mst_edges[to].push_back(from);

        isin[to][from] = true;
        isin[from][to] = true;
        sum += edges[to][from];
    }

    std::vector<std::vector<int>> mst_max(n, std::vector<int>(n));
    for (int i = 0; i < n - 1; i++) {
        //std::vector<bool> visited(n, false);
        //dfs(i, i, n, 0, edges, mst_edges, visited, mst_max);

        std::vector<uint8_t> visited(n);
        std::stack<std::tuple<int, int, int>> stack; // vertex idx, neigbor iter, current longest_edge
        stack.emplace(i, -1, 0);

        while (!stack.empty()) {
            auto& [vertex_idx, neigbor_iter, current_max] = stack.top();
            neigbor_iter++;
            if (neigbor_iter == (int)mst_edges[vertex_idx].size()) {
                stack.pop();
                continue;
            }

            const int next_neigbor = mst_edges[vertex_idx][neigbor_iter];
            if (visited[next_neigbor]) {
                continue;
            }

            const int next_max = std::max(current_max, edges[vertex_idx][next_neigbor]);
            mst_max[i][next_neigbor] = mst_max[next_neigbor][i] = next_max;
            visited[next_neigbor] = true;
            stack.emplace(next_neigbor, -1, next_max);
        }
    }

    int min = std::pow(2, 21);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            //std::cout << mst_max[i][j] << " ";
            if (!isin[i][j]) {
                min = std::min(min, edges[i][j] - mst_max[i][j]);
            }
        }
        //std::cout << "\n";
    }
    std::cout << sum + min << "\n";
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