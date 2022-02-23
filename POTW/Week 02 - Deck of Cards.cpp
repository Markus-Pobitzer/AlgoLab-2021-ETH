#include <iostream>
#include <vector>

/*
  Have a start vaiable where the current sum tart and go over every position once.
  Sum up the new values and whenever the sum > k increase the start poition until
  the curent end index is reached or the sum is < k. Store the first iterval that
  minimizes std::abs(k - sum).

  O(n)
*/

void testCase() {
    int n, k;
    std::cin >> n >> k;
    std::vector<int> inp(n);
    int start = 0;
    long sum = 0;
    std::vector<int> min = { k, 0, 0 };

    for (int i = 0; i < n; i++) {
        std::cin >> inp[i];
    }

    for (int i = 0; i < n; i++) {
        sum += inp[i];
        if (sum == k) {
            std::cout << start << " " << i << "\n";
            return;
        }
        if (std::abs(k - sum) < min[0]) {
            min[0] = std::abs(k - sum);
            min[1] = start;
            min[2] = i;
        }

        while (sum > k && start < i) {
            sum -= inp[start];
            start++;
            if (sum == k) {
                std::cout << start << " " << i << "\n";
                return;
            }
            if (std::abs(k - sum) < min[0]) {
                min[0] = std::abs(k - sum);
                min[1] = start;
                min[2] = i;
            }
        }
    }
    std::cout << min[1] << " " << min[2] << "\n";
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