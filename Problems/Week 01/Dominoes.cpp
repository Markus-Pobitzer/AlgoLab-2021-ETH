#include <iostream>
#include <vector>

int testCase() {
    int s; std::cin >> s;
    std::vector<int> out(s, s);
    std::vector<int> h(s, 0);
    out[0] = 0;
    for (int pos = 0; pos < s; pos++) {
        int hv; std::cin >> hv;
        h[pos] = hv;
    }
    int pm = 0;
    for (int pos = 0; pos < s; pos++) {
        int hv = h[pos];
        for (int i = std::max(0, pm - pos); i < hv && pos + i < s; i++) {
            if (out[pos + i] > out[pos]) {
                out[pos + i] = out[pos];
            }
            else {
                if (out[pos] > 0) {
                    return pos;
                }
                // out[pos + i] = out[pos];
            }
        }
        pm = std::max(pm, pos + hv - 1);
        //std::cout << out[pos] << " ";
    }
    return s;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        std::cout << testCase() << std::endl;
    }
}