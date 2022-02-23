#include <iostream>
#include <algorithm>
#include <vector>

void testCase() {
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> inp(n);
    for (int i = 0; i < n; i++) {
        int len, start;
        std::cin >> len >> start;
        inp[i] = std::make_pair(start, len);
    }

    std::sort(inp.begin(), inp.end());

    int count, pos, previousPos;
    count = 1;
    pos = inp[0].first;
    previousPos = pos;
    for (int i = 1; i < n; i++) {
        if (pos <= inp[i].first) {
            previousPos = pos;
            pos = std::max(inp[i].first, inp[i].second + pos);
            count++;
        }
        else if (std::max(inp[i].first, inp[i].second + previousPos) < pos) {
            pos = std::max(inp[i].first, inp[i].second + previousPos);
        }
    }

    std::cout << count << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}