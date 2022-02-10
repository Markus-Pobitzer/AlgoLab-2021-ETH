///4
#include <iostream>
#include <vector>
#include <algorithm>

/*
  Binary search over the possible number of Gulps. Every new position calls a
  function that check if we can reach Panoramix with #gulps. For that we make
  a split of the problem. We caluclate all possible subsets of the first half
  of moves [0, n/2[ and the second half [n/2, n[.
  In each recurive call we check if the current time < T. Than if the current
  pos == end and we have a distance of > 0 we add {time, dist} to our return
  vctor. otherwise we call the same fct once taking the current move and once
  skipping the move.

  We store the time and dist in a build struct with and override the < operator.
  It seems like that a vector of vector of long is in this caase too slow. I
  think the sorting takes longer with the vector than with the struct.

  Now we sort both our split vectors we got from the recursive fct. Starting from
  the beggining of the vectors we set the dist of the current vector to the max
  of the current distance and the one previously. This is important s.t. we can
  make a succesfull binary search in the next step.

  We check if the last entries of the vectors alone are >= D and return true. Otherwise
  we iterate over all the elements of the first vector and with binary search look
  at the last element in the second list which added time is < T. If th current
  dist plus the distance of the found element >= D we return true. If no such
  element is found return flase.

*/

std::vector<std::vector<long>> move;
std::vector<long> gulps;
int n, m;
long D, T;

struct my {
    long tim;
    long dist;

    bool operator < (const my& e1) {
        return (tim < e1.tim);
    }
};


void getTimDis(int pos, const int end, const int gulp, long dist, long tim, std::vector<my>& memo) {
    if (tim >= T)
        return;

    //std::cout << gulp << ": " << pos - 1 << " " << tim << " " << dist << "\n";

    if (pos >= end) {
        if (dist > 0)
            memo.push_back({ tim, dist });
        return;
    }

    // Do not take it
    getTimDis(pos + 1, end, gulp, dist, tim, memo);
    // Take it
    getTimDis(pos + 1, end, gulp, dist + move[pos][0] + gulps[gulp], tim + move[pos][1], memo);
}

bool isFast(int gulp) {
    std::vector<my> memo1;
    std::vector<my> memo2;

    getTimDis(0, n / 2, gulp, 0, 0, memo1);
    getTimDis(n / 2, n, gulp, 0, 0, memo2);

    std::sort(memo1.begin(), memo1.end());
    std::sort(memo2.begin(), memo2.end());

    int s1 = memo1.size();
    int s2 = memo2.size();

    // Prepare for binary search
    for (int i = 1; i < s1; i++) {
        memo1[i].dist = std::max(memo1[i - 1].dist, memo1[i].dist);
    }

    for (int i = 1; i < s2; i++) {
        memo2[i].dist = std::max(memo2[i - 1].dist, memo2[i].dist);
    }

    if (memo1[s1 - 1].dist >= D || memo2[s2 - 1].dist >= D)
        return true;

    // Iterat over all elements in first vector and see if a element in the second
    // vector fulfills the rrequiements. Use binary search.
    for (int i = 0; i < s1; i++) {
        int s = -1;
        int e = s2;
        while (s + 1 < e) {
            int middle = (e + s) / 2;
            if (memo2[middle].tim + memo1[i].tim < T)
                s = middle;
            else
                e = middle;
        }

        if (s >= 0 && memo1[i].dist + memo2[s].dist >= D) {
            return true;
        }

    }
    return false;
}

void testCase() {
    std::cin >> n >> m >> D >> T;

    move = std::vector<std::vector<long>>(n, std::vector<long>(2));
    gulps = std::vector<long>(m + 1);

    for (int i = 0; i < n; i++) {
        std::cin >> move[i][0] >> move[i][1];
    }

    gulps[0] = 0;
    for (int i = 1; i <= m; i++) {
        std::cin >> gulps[i];
    }

    int s = -1;
    int e = m + 1;

    // Do a binary search over all possible gulps we can take
    while (s + 1 < e) {
        int middle = (s + e) / 2;
        if (isFast(middle)) {
            e = middle;
        }
        else {
            s = middle;
        }
    }

    // If we have found a possible goulp s < m that fulfills the constraints
    if (s < m) {
        std::cout << (s + 1) << "\n";
    }
    else
        std::cout << "Panoramix captured\n";
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