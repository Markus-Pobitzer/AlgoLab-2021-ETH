#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

void testCase(int n) {
    long a; std::cin >> a;
    long b; std::cin >> b;
    long c; long d;
    K::Point_2 s(a, b);

    std::cin >> a;
    std::cin >> b;
    K::Point_2 e(a, b);

    K::Ray_2 ray(s, e);

    bool boo = false;
    for (int i = 0; i < n; i++) {
        std::cin >> a;
        std::cin >> b;

        std::cin >> c;
        std::cin >> d;

        if (!boo) {
            K::Point_2 segs(a, b);
            K::Point_2 sege(c, d);

            K::Segment_2 seg(segs, sege);
            if (CGAL::do_intersect(ray, seg)) {
                boo = true;
            }
        }
    }

    if (boo) {
        std::cout << "yes" << std::endl;
    }
    else {
        std::cout << "no" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t = 1;
    while (t > 0) {
        std::cin >> t;
        if (t > 0) {
            testCase(t);
        }
    }
}