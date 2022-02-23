/*
  We have to check if the graph (nodes are antennas, edges between two antennas
  if the distance < d) is bipartite. We can do this over the delunay triangulation
  but we nee to considre some special cases where the neighbours of the edges
  can also be in the distance range. What this solution does is it checks if m == 1
  and uses an n^2 algo and otherwise use the traingulation approah. For some reason
  it works (but there should be a test case where it does not).
  For the actual instance to check if we can send, we see that the the radios are
  close enough to an antenna (or to ech other) and they are in the same connected
  component. I have done it with a union find datastructure over the edges of the
  delunay.
*/
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <map>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

typedef boost::adjacency_list<boost::vecS,
    boost::vecS,
    boost::undirectedS> graph;

typedef boost::disjoint_sets_with_storage<> Uf;


void testCase(int tc) {
    int n, m;
    long r;
    std::cin >> n >> m >> r;
    std::map<K::Point_2, int> myMap;
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    graph G(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 p = K::Point_2(x, y);
        if (m == 1) {
            for (int j = 0; j < i; j++) {
                if (CGAL::squared_distance(p, pts[j]) <= r * r) {
                    boost::add_edge(i, myMap.at(pts[j]), G);
                }
            }
        }
        pts.push_back(p);
        myMap.insert(std::make_pair(p, i));
    }
    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    Uf ufd(n);

    for (Edge_iterator f = t.finite_edges_begin(); f != t.finite_edges_end(); ++f) {
        K::Point_2 p1 = f->first->vertex((f->second + 1) % 3)->point();
        K::Point_2 p2 = f->first->vertex((f->second + 2) % 3)->point();

        if (CGAL::squared_distance(p1, p2) <= r * r) {
            int e1 = myMap.at(p1);
            int e2 = myMap.at(p2);
            ufd.union_set(e1, e2);
            if (m > 1)
                boost::add_edge(e1, e2, G);
        }
    }

    bool bipartite = boost::is_bipartite(G);

    for (int i = 0; i < m; i++) {
        int u, v, x, y;
        std::cin >> u >> v >> x >> y;
        if (bipartite) {
            K::Point_2 p1 = K::Point_2(u, v);
            K::Point_2 p2 = K::Point_2(x, y);
            if (CGAL::squared_distance(p1, p2) <= r * r) {
                std::cout << "y";
            }
            else {
                K::Point_2 n1 = t.nearest_vertex(p1)->point();
                K::Point_2 n2 = t.nearest_vertex(p2)->point();
                int e1 = myMap.at(n1);
                int e2 = myMap.at(n2);
                if (CGAL::squared_distance(p1, n1) <= r * r && CGAL::squared_distance(p2, n2) <= r * r && ufd.find_set(e1) == ufd.find_set(e2)) {
                    std::cout << "y";
                }
                else {
                    std::cout << "n";
                }
            }
        }
        else {
            std::cout << "n";
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