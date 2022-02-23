
#include <iostream>
#include <vector>
#include <algorithm>

int f(int s, int over, int n, const std::vector<int>& timeToPos, std::vector<bool>& visited) {
    int ret = 0;
    if (!visited[s]) {
        ret = -1;
        if (timeToPos[s] >= over + 1) {
            if (s < (n - 1) / 2) {
                int a, b;
                a = f(2 * s + 1, over, n, timeToPos, visited);
                if (a >= 0) {
                    b = f(2 * s + 2, over + a, n, timeToPos, visited);
                    if (b >= 0 && timeToPos[s] >= over + a + b + 1) {
                        ret = a + b + 1;
                        visited[s] = true;
                    }
                }
            }
            else {
                ret = 1;
                visited[s] = true;
            }
        }
    }
    return ret;
}

void testCase() {
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> timeToPos(n);
    std::vector<bool> visited(n, false);
    std::vector<int> times(n);

    for (int i = 0; i < n; i++) {
        int ti; std::cin >> ti;
        timeToPos[i] = std::make_pair(ti, i);
        times[i] = ti;
    }

    std::sort(timeToPos.begin(), timeToPos.end());

    int ret = 0;
    for (int i = 0; ret >= 0 && i < n; i++) {
        if (!visited[timeToPos[i].second] && ret < timeToPos[i].first) {
            //std::cout << timeToPos[i].second << ": " << timeToPos[i].first << " with " << ret << "\n";
            int rb = f(timeToPos[i].second, ret, n, times, visited);
            if (rb >= 0) {
                ret += rb;
                //std::cout << ret << ": " << rb << "\n\n";
            }
            else {
                ret = -1;
            }
        }
    }
    if (ret < 0) {
        std::cout << "no\n";
    }
    else {
        std::cout << "yes\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}