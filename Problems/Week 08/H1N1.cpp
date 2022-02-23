#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <queue>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<long, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;

/*
  Infected are Delunay. Compute a face->info() where we use a priorit< queue to
  work from the infinit< edges with value 2^50 to the neighbouring people where
  we set as info in the queue the min of distance of the two connecting edges
  and the current info(). Than for a helthy person we locate the nearest infected
  see if it is far enough away, i.e. the distance >= d and also the info() of the
  located face is >= 4 * d.
*/

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
    std::priority_queue<std::pair<long, Triangulation::Face_handle>> pq;
    long max = std::pow(2, 60);
    // process all Voronoi vertices
    for (Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
        if (t.is_infinite(f)) {
            pq.push(std::make_pair(max, f));
            f->info() = max;
        }
        else {
            f->info() = -1;
        }
    }

    while (!pq.empty()) {
        std::pair<long, Triangulation::Face_handle> face = pq.top();
        pq.pop();
        if (face.second->info() < 0) {
            face.second->info() = face.first;
        }
        for (int i = 0; i < 3; i++) {
            auto ni = face.second->neighbor(i);
            if (ni->info() < 0) {
                long inp = CGAL::squared_distance(face.second->vertex((i + 1) % 3)->point(), face.second->vertex((i + 2) % 3)->point());
                inp = std::min(inp, face.second->info());
                pq.push(std::make_pair(inp, ni));
            }
        }
    }
    int m;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        int x, y;
        long d;
        std::cin >> x >> y >> d;
        K::Point_2 person = K::Point_2(x, y);
        long distance = CGAL::squared_distance(person, t.nearest_vertex(person)->point());
        auto o = t.locate(person);
        //std::cout << "\n" << person << ": " << d << " with " << distance << " and " << o->info() << "\n";
        if (distance >= d && o->info() >= 4 * d) {
            std::cout << "y";
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
    do {
        testCase(t);
        std::cin >> t;
    } while (t);
    return 0;
}