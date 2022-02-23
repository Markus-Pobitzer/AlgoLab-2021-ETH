#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;


void testCase(int n)
{
    // read number of points
    int l, b, r, t;
    std::cin >> l >> b >> r >> t;
    // construct triangulation
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    std::vector<int> firstD;
    firstD.reserve(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        pts.push_back(K::Point_2(x, y));
    }
    // construct triangulation
    Triangulation tr;
    tr.insert(pts.begin(), pts.end());

    // output all triangles
    //for (Edge_iterator f = tr.finite_edges_begin(); f != tr.finite_edges_end(); ++f){
    for (auto v = tr.vertices_begin(); v != tr.vertices_end(); ++v) {
        long xi = long(v->point().x());
        long yi = long(v->point().y());

        auto nearest = CGAL::nearest_neighbor(tr, v->handle());

        double maxRadius = std::min(std::min(std::abs(xi - l), std::abs(xi - r)), std::min(std::abs(yi - b), std::abs(yi - t)));

        //std::cout << "Radius: " << maxRadius << "\n";
        if (nearest != nullptr) {
            long xj = long(nearest->point().x());
            long yj = long(nearest->point().y());

            // Squared distance to neighbor
            long squaredDistance = (xi - xj) * (xi - xj) + (yi - yj) * (yi - yj);
            double nM = sqrt(squaredDistance) / 2.0;
            maxRadius = std::min(maxRadius, nM);
            //std::cout << "Radius 2: " << nM << "\n";
        }
        if (maxRadius > 0.5)
            maxRadius = std::ceil(std::sqrt(maxRadius - 0.5));
        else
            maxRadius = 0;
        firstD.push_back((int)maxRadius);
    }

    std::sort(firstD.begin(), firstD.end());

    std::cout << firstD[0] << " " << firstD[n / 2] << " " << firstD[n - 1] << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    do {
        testCase(n);
        std::cin >> n;
    } while (n != 0);
}
