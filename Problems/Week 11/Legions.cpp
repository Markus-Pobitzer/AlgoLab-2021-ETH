#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void testCase() {
    int xs, ys, n;
    std::cin >> xs >> ys >> n;

    std::vector<std::vector<long>> inp(n, std::vector<long>(4));
    for (int i = 0; i < n; i++) {
        long v;
        std::cin >> inp[i][0] >> inp[i][1] >> inp[i][2] >> v;
        long division = std::sqrt(inp[i][0] * inp[i][0] + inp[i][1] * inp[i][1]) * v;
        inp[i][3] = division;
    }

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp(CGAL::SMALLER, false, 0, false, 0);

    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;
    const int R = 2;

    for (int i = 0; i < n; i++) {
        if (inp[i][0] * xs + inp[i][1] * ys + inp[i][2] < 0) {
            lp.set_a(X, i, inp[i][0]); lp.set_a(Y, i, inp[i][1]); lp.set_b(i, -inp[i][2]);
        }
        else {
            lp.set_a(X, i, -inp[i][0]); lp.set_a(Y, i, -inp[i][1]); lp.set_b(i, inp[i][2]);
        }
        lp.set_a(R, i, inp[i][3]);
    }
    lp.set_l(R, 0);

    // objective function
    lp.set_c(R, -1);

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        std::cout << "Problem\n";
    }
    else {
        std::cout << floor_to_double(-s.objective_value()) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
    return 0;
}