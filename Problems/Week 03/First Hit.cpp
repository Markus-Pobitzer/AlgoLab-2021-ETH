#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

long floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return (long)a;
}

void testCase(int t) {
    long si; std::cin >> si;
    long ei; std::cin >> ei;
    K::Point_2 s(si, ei);
    std::cin >> si;
    std::cin >> ei;
    K::Point_2 e(si, ei);
    K::Ray_2 r(s, e);
    K::Line_2 rl(s, e);
    //std::cout << "Line: " << rl << ": " << s << " " << e << std::endl;

    bool b = false;
    K::FT distance;
    K::Point_2 ret;
    for (long i = 0; i < t; i++) {
        long lsx, lsy, lex, ley;
        std::cin >> lsx;
        std::cin >> lsy;
        std::cin >> lex;
        std::cin >> ley;

        //std::cout << lsx << " " << lsy << " " << lex << " " << ley << "\n";

        K::Line_2 line(K::Point_2(lsx, lsy), K::Point_2(lex, ley));
        K::Segment_2 seg(K::Point_2(lsx, lsy), K::Point_2(lex, ley));
        //if(rl.a() * line.b() != rl.b() * line.a()){
        if (CGAL::do_intersect(r, seg)) {
            //auto x = (-rl.c() * line.b() + line.c() * rl.b()) / (rl.a() * line.b() - line.a() * rl.b());
            //auto y = (-rl.a() * line.c() + line.a() * rl.c()) / (rl.a() * line.b() - line.a() * rl.b());
            //if(r.has_on(K::Point_2(x, y))){
            //}
            auto inter = CGAL::intersection(r, seg);
            if (const K::Point_2* op = boost::get<K::Point_2>(&*inter)) {
                if (!b) {
                    distance = CGAL::squared_distance(s, *op);
                    ret = *op;
                    b = true;
                }
                else {
                    if (distance > CGAL::squared_distance(s, *op)) {
                        distance = CGAL::squared_distance(s, *op);
                        ret = *op;
                    }
                }
            }
            else if (const K::Segment_2* sp = boost::get<K::Segment_2>(&*inter)) {
                if (!b) {
                    distance = CGAL::squared_distance(s, sp->source());
                    ret = sp->source();
                    b = true;
                }
                else {
                    if (distance > CGAL::squared_distance(s, sp->source())) {
                        distance = CGAL::squared_distance(s, sp->source());
                        ret = sp->source();
                    }
                }
                if (distance > CGAL::squared_distance(s, sp->target())) {
                    distance = CGAL::squared_distance(s, sp->target());
                    ret = sp->target();
                }
            }
        }
    }
    if (b) {
        //std::cout << (long)std::floor(CGAL::to_double(ret.x())) << " " << (long)std::floor(CGAL::to_double(ret.y())) << std::endl;
        std::cout << floor_to_double(ret.x()) << " " << floor_to_double(ret.y()) << std::endl;
    }
    else {
        std::cout << "no" << std::endl;
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    long t = 1;
    while (t > 0) {
        std::cin >> t;
        if (t > 0)
            testCase(t);
    }
}