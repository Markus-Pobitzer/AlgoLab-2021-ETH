/*
  Use Triangulation and disjoint sets.
  Fist iterate over the sorted edges in the traingulation and union_set all edges
  that have squared distane <= p. based on this we can iterate over all the missions
  and see if the 4 * max_squared distance of the mission start/end point is <= p.
  If this is the case save the point, (1) output "y" or "n" accordingly, and keep
  track of the max distance for all the saved points. At the same time keep track
  of all the points and their max distance.

  For (2) start wit the max_squared distance of all the points, iterate over all
  the sorted edges and union_set the two edges. If the current squred distance
  exeed the stored max update it. In eveery step we check if we can add a new
  mission as possible witha while loop. Keep in mind that we only check for the
  next mission and only if the next one is possible go on with the other missions.
  Output the updtated max distance.

  For (3) it is similar as with (2) but our max distance is different and we are
  only concerned about the points that made it in (1).

*/

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index, Index, K::FT> Edge;
typedef std::vector<Edge> EdgeV;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
    return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

void testCase() {
    Index n, m;
    long p;
    std::cin >> n >> m >> p;

    // read points: first, we read all points and store them into a vector,
    // together with their indices
    typedef std::pair<K::Point_2, Index> IPoint;
    std::vector<IPoint> points;
    points.reserve(n);
    for (Index i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points.emplace_back(K::Point_2(x, y), i);
    }
    // then we build the Delaunay triangulation in one shot, so as to leave the
    // choice of an efficient insertion order to the triangulation structure. By
    // giving the points paired with the indices, these indices are used to
    // initialize the vertex info accordingly.
    // This step takes O(n log n) time (for constructing the triangulation).
    Delaunay t;
    t.insert(points.begin(), points.end());

    // extract edges and sort by (squared) length
    // This step takes O(n log n) time (for the sorting).
    EdgeV edges;
    edges.reserve(3 * n); // there can be no more in a planar graph
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index i1 = e->first->vertex((e->second + 1) % 3)->info();
        Index i2 = e->first->vertex((e->second + 2) % 3)->info();
        // ensure smaller index comes first
        if (i1 > i2) std::swap(i1, i2);
        edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(),
        [](const Edge& e1, const Edge& e2) -> bool {
            return std::get<2>(e1) < std::get<2>(e2);
        });

    // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
    // in theory; for all practical purposes alpha(n) is constant, so linear time.

    // setup and initialize union-find data structure
    boost::disjoint_sets_with_storage<> uf(n);
    // ... and process edges in order of increasing length
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && std::get<2>(*e) <= p; ++e) {
        // determine components of endpoints
        Index c1 = uf.find_set(std::get<0>(*e));
        Index c2 = uf.find_set(std::get<1>(*e));
        if (c1 != c2) {
            // this edge connects two different components => part of the emst
            uf.link(c1, c2);
        }
    }

    std::vector<std::vector<long>> allM(m);
    std::vector<std::vector<long>> possibleM;
    long maxDA = 0;
    long maxPo = 0;
    for (int i = 0; i < (int)m; i++) {
        int sx, sy, tx, ty;
        std::cin >> sx >> sy >> tx >> ty;
        K::Point_2 ps(sx, sy);
        K::Point_2 pt(tx, ty);
        auto a = t.nearest_vertex(ps);
        auto b = t.nearest_vertex(pt);
        //std::cout << a->info() << " " << b->info() << "\n";
        long maxl = 4 * std::max(CGAL::squared_distance(ps, a->point()), CGAL::squared_distance(pt, b->point()));
        std::vector<long> local = { (long)a->info(), (long)b->info(), maxl };
        allM[i] = local;
        maxDA = std::max(maxDA, maxl);

        if (maxl <= p && uf.find_set(a->info()) == uf.find_set(b->info())) {
            std::cout << "y";
            possibleM.push_back(local);
            maxPo = std::max(maxPo, maxl);
        }
        else {
            std::cout << "n";
        }
    }
    std::cout << "\n";



    boost::disjoint_sets_with_storage<> uf2(n);
    // ... and process edges in order of increasing length
    int index = 0;
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && index < (int)m; ++e) {
        while (index < (int)m && uf2.find_set(allM[index][0]) == uf2.find_set(allM[index][1])) {
            index++;
        }

        if (index < (int)m) {
            if (std::get<2>(*e) > maxDA)
                maxDA = std::get<2>(*e);
            // determine components of endpoints
            Index c1 = uf2.find_set(std::get<0>(*e));
            Index c2 = uf2.find_set(std::get<1>(*e));
            if (c1 != c2) {
                // this edge connects two different components => part of the emst
                uf2.link(c1, c2);
            }

            while (index < (int)m && uf2.find_set(allM[index][0]) == uf2.find_set(allM[index][1])) {
                index++;
            }
        }
    }

    std::cout << maxDA << "\n";

    boost::disjoint_sets_with_storage<> uf3(n);
    // ... and process edges in order of increasing length
    index = 0;
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end() && index < (int)possibleM.size(); ++e) {
        while (index < (int)possibleM.size() && uf3.find_set(possibleM[index][0]) == uf3.find_set(possibleM[index][1])) {
            index++;
        }
        if (index < (int)possibleM.size()) {
            if (std::get<2>(*e) > maxPo)
                maxPo = std::get<2>(*e);
            // determine components of endpoints
            Index c1 = uf3.find_set(std::get<0>(*e));
            Index c2 = uf3.find_set(std::get<1>(*e));
            if (c1 != c2) {
                // this edge connects two different components => part of the emst
                uf3.link(c1, c2);
            }

            while (index < (int)possibleM.size() && uf3.find_set(possibleM[index][0]) == uf3.find_set(possibleM[index][1])) {
                index++;
            }
        }
    }
    std::cout << maxPo << "\n";

}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    std::size_t t;
    for (std::cin >> t; t > 0; --t) testCase();
    return 0;
}
