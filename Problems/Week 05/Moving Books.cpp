#include <iostream>
#include <vector>
#include <algorithm>

/*
  Sort books and persons, see if the strongest person can carry the heaviest book.
  With binary search get the start index of each person in the book array.
  Start from the strongest person and always let it carry the next availabe book
  if any. Than update the current index accordingly and also update the next pesons
  index with min(persons_ind, current_in) s.t. we actually travers the books array
  only once.
*/

void testCase() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> persons(n);
    std::vector<int> books(m);

    for (int i = 0; i < n; i++) {
        std::cin >> persons[i];
    }

    for (int i = 0; i < m; i++) {
        std::cin >> books[i];
    }

    std::sort(books.begin(), books.end());
    std::sort(persons.begin(), persons.end());

    if (persons[n - 1] < books[m - 1]) {
        std::cout << "impossible\n";
    }
    else {
        std::vector<bool> visited(m, false);
        std::vector<int> index(n);
        for (int i = 0; i < n; i++) {
            if (persons[i] < books[0]) {
                index[i] = -1;
            }
            else {
                int l = -1;
                int r = m;
                while (l + 1 < r) {
                    int m = (l + r) / 2;
                    if (books[m] > persons[i]) {
                        r = m;
                    }
                    else {
                        l = m;
                    }
                }
                index[i] = r - 1;
                //std::cout << index[i] << " ";
            }
        }
        // std::cout << std::endl;

        int startl = 0;
        int ret = -1;
        int count = 0;
        while (count < m) {
            ret += 1;
            for (int i = n - 1; i >= startl; i--) {
                int ind = index[i];
                while (ind >= 0 && visited[ind]) {
                    ind--;
                }
                if (ind < 0) {
                    startl = std::max(startl, i + 1);
                }
                else {
                    visited[ind] = true;
                    //std::cout << i << " moved book " << ind << std::endl;
                    count += 1;
                    ind--;
                }
                index[i] = ind;
                if (i > 0 && index[i - 1] > 0) {
                    index[i - 1] = std::min(index[i - 1], ind);
                }
            }
            ret += 2;
        }

        std::cout << ret << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
}