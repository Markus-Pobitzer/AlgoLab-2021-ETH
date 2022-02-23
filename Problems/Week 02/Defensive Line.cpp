#include <iostream>
#include <vector>
#include <list>

int f(int start, int k, int n, std::vector<std::vector<int>>& memo, std::vector<int>& pre) {
    int ret = 0;
    if (k > 0) {
        if (n < start + k) {
            ret = -1;
        }
        else {
            if (memo[start][k] >= -19) {
                ret = memo[start][k];
            }
            else {
                int val = pre[start];
                if (val < 0) {
                    ret = f(start + 1, k, n, memo, pre);
                }
                else {
                    int fb = f(start + val, k - 1, n, memo, pre);
                    int fw = f(start + 1, k, n, memo, pre);
                    ret = std::max(((fb < 0) ? -1 : (val + fb)), fw);
                    //std::cout << ret << " ";
                }
                memo[start][k] = ret;
            }
        }
    }
    return ret;
}

void tstCase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;

    std::vector<int> ds(n);
    for (int i = 0; i < n; i++) {
        std::cin >> ds[i];
    }

    std::vector<int> pre(n);
    int e = 0;
    int summe = 0;
    for (int i = 0; i < n; i++) {
        if (i != 0) {
            summe -= ds[i - 1];
        }
        while (summe < k && e < n) {
            summe += ds[e];
            e++;
        }
        if (summe == k) {
            pre[i] = e - i;
        }
        else {
            pre[i] = -1;
        }
        // std::cout << pre[i] << " ";
    }
    // std::cout << std::endl;

    std::vector<std::vector<int>> memo(n, std::vector<int>(m + 1, -20));
    for (int i = 0; i < n; i++) {
        memo[i][0] = 0;
    }
    int ret = f(0, m, n, memo, pre);
    /*
    for(int i = 0; i < n; i++){
      for(int j = 0; j <= m ;j++){
        std::cout << memo[i][j] << " ";
      }
      std::cout << std::endl;
    }*/

    if (ret >= 0) {
        std::cout << ret << std::endl;
    }
    else {
        std::cout << "fail" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        tstCase();
    }
}

int dfs(std::vector<std::list<int>>& graph, std::vector<std::pair<int, int>>& interv, int index, int k) {
    int ret = -1;
    if (k > 1) {
        for (int ind : graph[index]) {
            ret = std::max(ret, dfs(graph, interv, ind, k--));
        }
    }
    if (index > 0 && ret > -1) {
        ret += interv[index - 1].second - interv[index - 1].first + 1;
    }
    return ret;
}

void tstCaseOld() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;

    std::vector<int> ds(n);
    for (int i = 0; i < n; i++) {
        std::cin >> ds[i];
    }

    int j = 0;
    int sum = 0;
    std::list<std::pair<int, int>> interv;
    for (int i = 0; i < n; i++) {
        sum += ds[i];
        if (sum == k) {
            interv.push_back(std::pair<int, int>(i, j));
        }
        else {
            while (sum > k && j < i) {
                sum -= ds[j];
                j++;
            }
            if (sum == k) {
                interv.push_back(std::pair<int, int>(i, j));
            }
        }
    }

    std::vector<int> begin(interv.size() + 1, 2000000);
    begin[0] = -1;

    int l = 1;
    for (std::pair<int, int> p : interv) {
        begin[l] = p.second;
        l++;
    }

    // Here comes the slow down
    std::vector<std::list<int>> graph(interv.size() + 1);
    int ind = 0;
    int con = 1;
    for (std::pair<int, int> p : interv) {
        while (ind <= (int)interv.size() && p.first > begin[ind]) {
            ind++;
        }
        for (int i = 0; i < ind; i++) {
            graph[i].push_back(con);
        }
        con++;
    }

    std::vector<std::pair<int, int>> intervals(interv.size());
    int o = 0;
    for (std::pair<int, int> p : interv) {
        intervals[o] = p;
        o++;
    }
    // DFS
    int ret = dfs(graph, intervals, 0, k + 1);
    if (ret == -1) {
        std::cout << "fail" << std::endl;
    }
    else {
        std::cout << ret << std::endl;
    }
}