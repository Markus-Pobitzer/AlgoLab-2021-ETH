///2
#include <iostream>
#include <math.h>
#include <map>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

/*
  Realizing h = the radius of the lamps and if no person survives all lamps we print
  these persons who got eliminated last. Delunay on the lamps. for each person check
  if a lamp hits it (neares vertex), if this is the case and no other person completly
  survived all lmps go over all previous lamps and see if current person got hit
  previoulsy by a lamp (used priority queue and index).
*/

void testCase(int tc) {
    int m, n;
    long h;
    std::cin >> m >> n;
    std::vector<K::Point_2> points(m);
    std::vector<long> radius(m);
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = K::Point_2(x, y);
        std::cin >> radius[i];
    }

    std::cin >> h;
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    std::map<K::Point_2, int> myMap;
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
        myMap.insert(std::make_pair(K::Point_2(x, y), i));
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    int index = 0;
    std::priority_queue<int, std::vector<int>, std::greater<int>> q;

    for (int i = 0; i < m; i++) {
        K::Point_2 p = points[i];
        long dist = CGAL::squared_distance(p, t.nearest_vertex(p)->point());

        if (tc == -1 && i == 2) {
            std::cout << p << ": " << dist << " " << std::pow(h + radius[i], 2) << "\n";
        }
        long maxD = std::pow(h + radius[i], 2);
        if (dist >= maxD) {
            std::cout << i << " ";
            index = -1;
        }
        else if (index >= 0) {
            int lind = myMap.at(t.nearest_vertex(p)->point());
            for (int j = lind; j >= 0; j--) {
                if (CGAL::squared_distance(p, pts[j]) < maxD) {
                    lind = j;
                }
            }
            if (tc == -1 && i == 2) {
                std::cout << "Lind: " << lind << " index: " << index << "\n";
            }
            if (lind == index) {
                q.push(i);
            }
            else if (lind > index) {
                q = std::priority_queue<int, std::vector<int>, std::greater<int>>();
                index = lind;
                q.push(i);
            }
        }
    }
    if (index >= 0) {
        while (!q.empty()) {
            std::cout << q.top() << " ";
            q.pop();
        }
    }
    std::cout << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase(t);
    }
    return 0;
}