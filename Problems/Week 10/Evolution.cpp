#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

/*
  First we store the whole correlation as a tree. Where every entry points to its
  children nodes. We go over all queries and store for each vertex the associated
  queries in a vector.
  Now we go with a DFS over the tree and build/tear down the current path in each
  recursive step. In one step of the DFS we also go over all the queries of this
  vertex and make a binary search on the current path that contains only relevant
  vertices.
*/
void dfs(int ind, std::vector<int>& path, const std::vector<std::vector<std::pair<int, int>>>& queries, const std::vector<std::vector<int>>& tree, const std::vector<std::vector<int>>& ages, std::vector<int>& results) {
    path.push_back(ind);

    for (int i = 0; i < (int)queries[ind].size(); i++) {
        std::pair<int, int> q = queries[ind][i];

        // Bianry search
        int l = -1; // the first leftmost position where it can not be
        int r = (int)path.size() - 1;
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (ages[path[m]][0] > q.first) {
                l = m;
            }
            else {
                r = m;
            }
        }

        results[q.second] = path[r];
    }

    for (int i = 0; i < (int)tree[ind].size(); i++) {
        dfs(tree[ind][i], path, queries, tree, ages, results);
    }

    path.pop_back();
}

void testCase(int tc) {
    int n, q;
    std::cin >> n >> q;

    std::vector<std::string> names(n);
    std::vector<std::vector<int>> ages(n, std::vector<int>(2));

    for (int i = 0; i < n; i++) {
        std::cin >> names[i] >> ages[i][0];
        ages[i][1] = i;
    }
    std::sort(ages.begin(), ages.end(), std::greater<std::vector<int>>());
    std::map<std::string, int> m;

    // Maps name to the index of ages
    for (int i = 0; i < n; i++) {
        m.insert(std::make_pair(names[ages[i][1]], i));
    }

    std::vector<std::vector<int>> tree(n);
    for (int i = 0; i < n - 1; i++) {
        std::string n1, n2;
        std::cin >> n1 >> n2;
        tree[m.at(n2)].push_back(m.at(n1));
    }

    std::vector<std::vector<std::pair<int, int>>> queries(n);
    for (int i = 0; i < q; i++) {
        std::string st; std::cin >> st;
        int maxAge; std::cin >> maxAge;
        queries[m.at(st)].push_back(std::make_pair(maxAge, i));
    }

    std::vector<int> results(q);
    std::vector<int> path;
    dfs(0, path, queries, tree, ages, results);

    for (int i = 0; i < q; i++) {
        std::cout << names[ages[results[i]][1]] << " ";
    }
    std::cout << "\n";

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
    return 0;
}