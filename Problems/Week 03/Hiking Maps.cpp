#include <iostream>
#include <vector>
#include <math.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <boost/optional/optional_io.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

bool isPrefix(int start, int len, int m, std::vector<std::vector<int>>& prefix) {
    bool ret = true;
    if (start == 0) {
        for (int i = 0; ret && i < m - 1; i++) {
            ret = ((prefix[len - 1][i] > 0) ? true : false);
        }
    }
    else {
        for (int i = 0; ret && i < m - 1; i++) {
            ret = (((prefix[start + len - 1][i] - prefix[start - 1][i]) > 0) ? true : false);
        }
    }
    return ret;
}

int cost(int start, int len, int n, int m, std::vector<std::vector<int>>& cmemo, std::vector<std::vector<int>>& prefix) {
    int ret = n;
    //std::cout << start << " " << len << std::endl;
    if (len > 0) {
        if (cmemo[start][len] == -1) {
            int local = n;
            if (isPrefix(start, len, m, prefix)) {
                local = len;
                //if(len < 17)
                //std::cout << start << " " << len << std::endl;
            }
            ret = std::min(cost(start, len - 1, n, m, cmemo, prefix), local);
            ret = std::min(ret, cost(start + 1, len - 1, n, m, cmemo, prefix));
            cmemo[start][len] = ret;
        }
        else {
            ret = cmemo[start][len];
        }
    }
    return ret;
}

void testCase() {
    int m; std::cin >> m; // legs
    int n; std::cin >> n; // triangels

    std::vector<K::Point_2> legs(m);
    std::vector<std::vector<K::Point_2>> triangle(n, std::vector<K::Point_2>(6));
    std::vector<std::vector<CGAL::Orientation>> orient(n, std::vector<CGAL::Orientation>(3));
    std::vector<std::vector<int>> prefix(n, std::vector<int>(m - 1, 0));


    for (int i = 0; i < m; i++) {
        int ex; std::cin >> ex;
        int ey; std::cin >> ey;
        legs[i] = K::Point_2(ex, ey);
    }
    for (int i = 0; i < n; i++) {
        //std::vector<K::Line_2> l(3);
        for (int j = 0; j < 3; j++) {
            int px0, py0, px1, py1;
            std::cin >> px0;
            std::cin >> py0;
            std::cin >> px1;
            std::cin >> py1;

            triangle[i][j * 2] = K::Point_2(px0, py0);
            triangle[i][j * 2 + 1] = K::Point_2(px1, py1);
            //l[j] = K::Line_2(K::Point_2(px0, py0), K::Point_2(px1, py1));
        }

        for (int j = 0; j < 3; j++) {
            orient[i][j] = CGAL::orientation(triangle[i][j * 2], triangle[i][j * 2 + 1], triangle[i][(j * 2 + 2) % 6]);
            if (orient[i][j] == 0) {
                std::cout << "Orientation: " << i << " " << j << std::endl;
            }
            //std::cout << orient[i][j] << " ";
        }
    }

    for (int i = 0; i < n; i++) { // triangle
        bool lt = true;
        for (int j = 0; j < m; j++) { // leg
            bool ct = true;
            //if(i == 0 && j == 4)
              //std::cout << "Punkt is : " << legs[j] << std::endl;
            for (int ti = 0; ct && ti < 3; ti++) {
                auto o1 = CGAL::orientation(triangle[i][ti * 2], triangle[i][ti * 2 + 1], legs[j]);
                /*if(i == 0 && j == 4){
                  std::cout << "TI: " << (ti * 2) << " " << (ti * 2 + 1) << ": " << triangle[i][ti * 2] << " and " << triangle[i][ti * 2 + 1] << std::endl;
                }*/
                if (o1 != 0 && orient[i][ti] != o1) {
                    ct = false;
                }
                /*
                if(i == 0 && (j == 4)){
                  std::cout << "j is " << j << " " << ti << ": " << orient[i][ti] << std::endl;
                  std::cout << "Leg: " << o1 << std::endl;
                }*/
            }
            //std::cout << i << " " << j << ": " << ct << std::endl;
            if (j > 0) {
                if (i > 0) {
                    prefix[i][j - 1] = prefix[i - 1][j - 1];
                    //std::cout << i << " " << j << ": " << prefix[i][j - 1] << std::endl;
                }
                if (lt && ct) {
                    prefix[i][j - 1] += 1;
                }
                /*
                // ====================================
                bool isit = false;
                K::Segment_2 seg(legs[j-1], legs[j]);

                K::Line_2 l1(triangle[i][0], triangle[i][1]);
                K::Line_2 l2(triangle[i][2], triangle[i][3]);
                K::Line_2 l3(triangle[i][4], triangle[i][5]);

                auto o0 = CGAL::intersection(l1, l2);
                auto o1 = CGAL::intersection(l1, l3);
                auto o2 = CGAL::intersection(l2, l3);

                K::Triangle_2 tri(*boost::get<K::Point_2>(&*o0), *boost::get<K::Point_2>(&*o1), *boost::get<K::Point_2>(&*o2));
                //std::cout << "Triangle: " << triangles[i] << std::endl;
                if(CGAL::do_intersect(tri, seg)){
                  auto segm = CGAL::intersection(tri, seg);
                  if(const K::Segment_2* os = boost::get<K::Segment_2>(&*segm)){
                    if(*os == seg){
                      isit = true;
                    }
                  }
                }
                if(isit != (lt && ct)){
                 //std::cout << "Wrong should be " << isit << ": " << i << " " << j << std::endl;
                }
                // ===========================================*
                */
            }
            lt = ct;
        }
    }
    /*
    for(int i = 0; i < n; i++){
      std::cout << i << "\t";
      for(int j = 0; j < m-1; j++){
        std::cout << prefix[i][j] << "  ";
      }
      std::cout << std::endl;
    }
    */

    int start = 0;
    int best = n;
    for (int i = 0; i < n; i++) {
        while (start <= i && isPrefix(start, i - start + 1, m, prefix)) {
            if (best > i - start + 1) {
                best = i - start + 1;
            }
            start++;
        }
    }
    std::cout << best << std::endl;


    //std::vector<std::vector<int>> cmemo(n + 1, std::vector<int>(n + 1, -1));
    //std::cout << cost(0, n, n, m, cmemo, prefix) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        //std::cout << "Test " << i << std::endl;
        testCase();
    }
}