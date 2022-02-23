#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>
// typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

long ceil_to_long(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return (long)a;
}

void testCase(int t) {
    std::vector<K::Point_2> P(t);
    for (int i = 0; i < t; ++i) {
        long s, e;
        std::cin >> s;
        std::cin >> e;
        P[i] = K::Point_2(s, e);
    }

    Min_circle mc(P.begin(), P.end(), true);
    Traits::Circle c = mc.circle();
    std::cout << ceil_to_long(CGAL::sqrt(c.squared_radius())) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t != 0) {
        testCase(t);
        std::cin >> t;
    }
    return 0;
}