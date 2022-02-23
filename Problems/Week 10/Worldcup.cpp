// example: decide whether two point sets R and B can be separated by a nonvertical line
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_long(const CGAL::Quotient<ET>& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void testCase() {
    int n, m, c;
    std::cin >> n >> m >> c;
    std::vector<K::Point_2> pW(n);
    std::vector<K::Point_2> pS(m);
    // std::vector<K::Points_2) pC(c);

    std::vector<int> s(n);
    std::vector<int> a(n);

    std::vector<int> d(m);
    std::vector<int> u(m);

    std::vector<std::vector<int>> rws(n, std::vector<int>(m));

    std::vector<std::vector<int>> nc(n, std::vector<int>(c));
    std::vector<std::vector<int>> mc(m, std::vector<int>(c));

    for (int i = 0; i < n; i++) {
        int x, y, si, ai;
        std::cin >> x >> y >> si >> ai;

        pW[i] = K::Point_2(x, y);
        s[i] = si;
        a[i] = ai;
    }

    for (int i = 0; i < m; i++) {
        int x, y, di, ui;
        std::cin >> x >> y >> di >> ui;

        pS[i] = K::Point_2(x, y);
        d[i] = di;
        u[i] = ui;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> rws[i][j];
        }
    }

    for (int i = 0; i < c; i++) {
        int x, y;
        long r;
        std::cin >> x >> y >> r;
        r = r * r;
        K::Point_2 p(x, y);
        for (int j = 0; j < n; j++) {
            nc[j][i] = (CGAL::squared_distance(p, pW[j]) <= r) ? 1 : 0;
        }

        for (int j = 0; j < m; j++) {
            mc[j][i] = (CGAL::squared_distance(p, pS[j]) <= r) ? 1 : 0;
        }
    }

    std::vector<std::vector<int>> tws(n, std::vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < c; k++) {
                tws[i][j] += (nc[i][k] + mc[j][k]) % 2;
            }
        }
    }


    // create an LP with Ax <= b and no lower/upper bounds
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    int counter = 0;
    //Variable index of pos (i, j): var = i * m + j

    // No more beer than supply
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_a(i * m + j, counter, 1);
        }
        lp.set_b(counter, s[i]);
        counter++;
    }

    // Litres in one stadium <= demand
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            lp.set_a(j * m + i, counter, 1);
        }
        lp.set_b(counter, d[i]);
        counter++;
    }

    // Litres in one stadium >= demand
    // a1s + ... + ans >= ds
    // - a1s - ... - ans <= -ds
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            lp.set_a(j * m + i, counter, -1);
        }
        lp.set_b(counter, -d[i]);
        counter++;
    }

    // Upper limit of alcohol in each stadium
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            lp.set_a(j * m + i, counter, a[j]);
        }
        lp.set_b(counter, 100 * u[i]);
        counter++;
    }


    // Min - sum of all profits - tws
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_c(i * m + j, -(100 * rws[i][j] - tws[i][j]));
        }
    }
    // solve the program, using ET as the exact type
    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) {
        std::cout << "RIOT!" << "\n";
    }
    else {
        std::cout << floor_to_long(-sol.objective_value() / 100) << "\n";
        //std::cout << sol << "\n";
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
    return 0;
}
