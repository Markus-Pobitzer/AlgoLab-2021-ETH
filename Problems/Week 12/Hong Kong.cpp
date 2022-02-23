///2
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

void testCase(int tc) {
    int n, m;
    long r;
    std::cin >> n >> m >> r;
    std::vector<K::Point_2> trees(n);

    for (int i = 0; i < n; i++) {
        long x, y;
        std::cin >> x >> y;
        trees[i] = K::Point_2(x, y);
    }

    // construct triangulation
    Triangulation t;
    t.insert(trees.begin(), trees.end());

    /*
    * For every face compute the maximal radius in the face, i.e. squared_radius(r, p, q)
    * or infinity if the face is inifinite.
    * Sort the faces accordingly.
    * Map for face to index, sort vector of lengths and faces.
    * Now from the sorted faces start from the biggest and go through all three
    * neighbouring faces (visit only if the face is not infinite and not already
    * visited, i.e. in a lowe order of th sorted list) and set the value of the face
    * to max(own_value, min(squared_distance(between points of the edge), the current_value))
    * From there we can query each baloon and see if it fits or not.
    *
    */
    K::FT maxV = K::FT(999999999999999) * K::FT(99999999999999) * K::FT(99999999999999) * K::FT(99999999999999);

    typedef std::pair<K::FT, Triangulation::Face_handle> my_pair;
    // const auto comp = [](const my_pair& p1, const my_pair& p2) -> bool {return p1.first < p2.first;};

    std::priority_queue<my_pair> qu;
    for (auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f) {
        f->info() = K::FT(-1);
        if (t.is_infinite(f)) {
            qu.push(std::make_pair(maxV, f));
        }
        else {
            K::FT pd = K::FT(CGAL::squared_radius(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point()));
            qu.push(std::make_pair(pd, f));
        }
    }

    while (!qu.empty()) {
        std::pair<K::FT, Triangulation::Face_handle> fs = qu.top();
        qu.pop();

        if (fs.second->info() < 0) {
            Triangulation::Face_handle f = fs.second;
            f->info() = fs.first;
            for (int i = 0; i < 3; i++) {
                if (!t.is_infinite(f->neighbor(i)) && f->neighbor(i)->info() < 0) {
                    K::FT smal = CGAL::squared_distance(f->vertex((i + 1) % 3)->point(), f->vertex((i + 2) % 3)->point());
                    qu.push(std::make_pair(std::min(smal, fs.first), f->neighbor(i)));
                }
            }
        }
    }
    /*
    for(int i = 0; i < index; i++){
      std::cout << max_to[i] << " ";
    }
    std::cout << "\n";*/

    for (int i = 0; i < m; i++) {
        long x, y, s;
        std::cin >> x >> y >> s;
        K::Point_2 b = K::Point_2(x, y);
        K::FT dist = K::FT(r + s) * K::FT(r + s);

        if (CGAL::squared_distance(b, t.nearest_vertex(b)->point()) < dist) {
            std::cout << "n";
        }
        else {
            if (t.locate(b)->info() < 4 * dist) {
                std::cout << "n";
            }
            else {
                std::cout << "y";
            }
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
}