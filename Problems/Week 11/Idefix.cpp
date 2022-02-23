///3
#include <iostream>
#include <vector>
#include <map>
#include <boost/pending/disjoint_sets.hpp>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <algorithm>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_edges_iterator Edge_iterator;

typedef boost::disjoint_sets_with_storage<> Uf;

/*
  Deluny triangulation in two steps. Create Delunay out of the Oak trees.
  1) For the first task we create a union find data structure for every tree
     and go over all the finite edges in the tree. If an edge (u,v) has squared
     length less or equals to s we union the two sets of u and v. Now for every
     bone we look at the neares tree, check if 4 * squared distance <= s and than
     add a plus one to the set where the neares tree lands. In the end we priint
     the set where the most trees landed.
  2) For the second task we create also a union find datastructure over the trees.
     But now we need two vectors, one that represents the distance of a bone to its
     closest tree (squared and times 4) and the corresponding tree, the length
     of every finite edge, and the edge.
     We sort these vectors accordingly and take as our initial radius the k-1
     entry of the bone distance, i.e. we say that we need at least k bones.
     From there on we add (count) all bones to the accordig disjoint sets, union
     all sets where we have an edge that is <= the radius and increase the radius
     to the next smaller possible entry in our wo vectors. We repeat this until one
     set counts at leaast k bones.
*/

void testCase() {
    int n, m, k;
    long s;
    std::cin >> n >> m >> s >> k;
    // read points
    std::vector<K::Point_2> pts;
    pts.reserve(n);

    std::map<K::Point_2, int> map;
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 p = K::Point_2(x, y);
        pts.push_back(p);
        map.insert(std::make_pair(p, i));
    }

    // construct triangulation
    Triangulation t;
    t.insert(pts.begin(), pts.end());

    Uf uf1(n);
    std::vector<int> count(n, 0);
    std::vector<std::pair<long, K::Point_2>> boneDist(m);
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        K::Point_2 p = K::Point_2(x, y);
        K::Point_2 nv = t.nearest_vertex(p)->point();
        long dist = CGAL::squared_distance(p, nv);

        boneDist[i] = std::make_pair(4 * dist, nv);
        if (4 * dist <= s) {
            count[map.at(nv)] += 1;
        }
    }

    std::vector<std::vector<long>> closestEdge;
    for (Edge_iterator f = t.finite_edges_begin(); f != t.finite_edges_end(); ++f) {
        K::Point_2 e1 = (*f).first->vertex(((*f).second + 1) % 3)->point();
        K::Point_2 e2 = (*f).first->vertex(((*f).second + 2) % 3)->point();

        long dist = CGAL::squared_distance(e1, e2);

        int index1 = map.at(e1);
        int index2 = map.at(e2);

        closestEdge.push_back({ dist, index1, index2 });
        if (dist <= s) {
            int i1 = uf1.find_set(index1);
            int i2 = uf1.find_set(index2);

            if (i1 != i2) {
                int sum = count[i1] + count[i2];
                uf1.union_set(i1, i2);
                count[i1] = sum;
                count[i2] = sum;
            }
        }
    }

    int max1 = 0;
    for (int i = 0; i < n; i++) {
        max1 = std::max(max1, count[i]);
    }

    Uf uf2(n);
    std::vector<int> count2(n, 0);
    std::sort(boneDist.begin(), boneDist.end());
    std::sort(closestEdge.begin(), closestEdge.end());

    /*for(int i = 0; i < m ; i++){
      std::cout << boneDist[i].first << "\n";
    }*/
    long radius = boneDist[k - 1].first;
    int ret = 0;

    int bi = 0;
    int ei = 0;
    int esize = (int)closestEdge.size();

    // std::cout << "YES: " << m << " edges: " << esize << std::endl;
    do {
        // std::cout << bi << " " << ei << std::endl;
        while (bi < m && boneDist[bi].first <= radius) {
            int i1 = uf2.find_set(map.at(boneDist[bi].second));
            count2[i1] += 1;
            ret = std::max(ret, count2[i1]);
            bi++;
        }
        while (ei < esize && closestEdge[ei][0] <= radius) {
            int i1 = uf2.find_set(closestEdge[ei][1]);
            int i2 = uf2.find_set(closestEdge[ei][2]);
            if (i1 != i2) {
                int sum = count2[i1] + count2[i2];
                ret = std::max(ret, sum);
                uf2.union_set(i1, i2);
                count2[i1] = sum;
                count2[i2] = sum;
            }
            ei++;
        }

        // std::cout << radius << ": " << ret << "/" << k << "\n";
        if (ret < k) {
            if (bi < m && ei < esize) {
                radius = std::min(boneDist[bi].first, closestEdge[ei][0]);
            }
            else if (bi < m) {
                radius = boneDist[bi].first;
            }
            else {
                radius = closestEdge[ei][0];
            }
        }
    } while (ret < k);

    std::cout << max1 << " " << radius << "\n";
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