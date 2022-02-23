#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

void testCase(int n) {
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 p = K::Point_2(x, y);
        Vertex_handle handle = t.nearest_vertex(p);
        K::Point_2 np = handle->point();
        //long dist = std::abs(x - np.x) + std::abs(y - np.y);
        long dist = CGAL::squared_distance(p, np);
        std::cout << dist << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    do {
        testCase(t);
        std::cin >> t;
    } while (t != 0);
}