#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <math.h>

void testCase() {
    int n; std::cin >> n;
    std::vector<int> s(n);
    std::list<int> loc;

    for (int i = 0; i < n; i++) {
        std::cin >> s[i];
    }

    std::sort(s.begin(), s.end());

    int maxC = 0; int pos = 0;
    int lC = 0; int lj = 0;

    for (int i = 0; i < n; i++) {
        lC++;
        while (s[lj] < s[i] - 200) {
            lC--;
            lj++;
        }
        if (lC > maxC) {
            maxC = lC;
            int place = (s[i] + s[lj]) / 2;
            pos = std::max(std::abs(s[i] - place), std::abs(s[lj] - place));
            loc.clear();
            //if(lC == 3){
              //std::cout << (i- lj + 1) << " gives " << lj << ": " << s[lj] << " and " << i << ": " << s[i] << std::endl;
            //}
            loc.push_back(place);
            if ((s[i] % 2 != 0 && s[lj] % 2 == 0) || (s[i] % 2 == 0 && s[lj] % 2 != 0)) {
                if (place < 0) {
                    loc.push_back(place - 1);
                }
                else {
                    loc.push_back(place + 1);
                }
            }
            //if(fmod((s[i] + s[lj]) / 2.0, 1.0) == 0.5){
              //loc.push_back(place + ((place < 0)? -1 : 1));
            //}
        }
        else if (lC == maxC) {
            int place = (s[i] + s[lj]) / 2;
            int lpos = std::max(std::abs(s[i] - place), std::abs(s[lj] - place));
            //if(lC == 3){
              //std::cout << (i- lj + 1) << " gives " << lj << ": " << s[lj] << " and " << i << ": " << s[i] << std::endl;
            //}
            if (lpos < pos) {
                pos = lpos;
                loc.clear();
            }
            if (lpos == pos) {
                loc.push_back(place);
                if ((s[i] % 2 != 0 && s[lj] % 2 == 0) || (s[i] % 2 == 0 && s[lj] % 2 != 0)) {
                    if (place < 0) {
                        loc.push_back(place - 1);
                    }
                    else {
                        loc.push_back(place + 1);
                    }
                }
            }
        }
    }

    std::cout << maxC << " " << pos << std::endl;
    loc.sort();
    for (int elem : loc) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;

    for (int i = 0; i < t; i++) {
        testCase();
    }
}