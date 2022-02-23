#include <iostream>
#include <vector>


int f(int start, int end, int k, int m, std::vector<int>& psum, std::vector<std::vector<int>>& memo) {
    int ret = 0;
    if (end - start >= k) {
        //if(k == 0){
          //ret = memo[start][end];
        //} else {
        ret = memo[start][end];
        //}

        //std::cout << "k: " << k << " " << ret << std::endl;
        if (ret < 0) {
            if (start == end) {
                ret = psum[end] - ((start == 0) ? 0 : psum[start - 1]);
            }
            else {
                int l = psum[start] - ((start == 0) ? 0 : psum[start - 1]);
                int r = psum[end] - ((end == 0) ? 0 : psum[end - 1]);
                if (k == 0) {
                    ret = std::max(l + f(start + 1, end, m - 1, m, psum, memo), r + f(start, end - 1, m - 1, m, psum, memo));
                }
                else {
                    ret = std::min(f(start + 1, end, k - 1, m, psum, memo), f(start, end - 1, k - 1, m, psum, memo));
                }
            }
            //if(k == 0){
              //memo[start][end] = ret;
            //} else {
            memo[start][end] = ret;
            //}
        }
    }
    return ret;
}

int testCase() {
    int n; std::cin >> n;
    int m; std::cin >> m;
    int k; std::cin >> k;

    std::vector<int> val(n);
    std::vector<int> psum(n);
    for (int i = 0; i < n; i++) {
        std::cin >> val[i];
        psum[i] = val[i] + ((i == 0) ? 0 : psum[i - 1]);
    }

    std::vector<std::vector<int>> memo(n, std::vector<int>(n, -1));
    int ret = f(0, n - 1, k, m, psum, memo);
    /*
    for(int i = 0; i < n; i++){
      for(int j = 0; j < n; j++){
        std::cout << memo[i][j] << "    ";
      }
      std::cout << std::endl;
    }
    */
    return ret;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        std::cout << testCase() << std::endl;
    }
}