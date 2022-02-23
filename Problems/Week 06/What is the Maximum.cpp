#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

double ceil_to_double(const CGAL::Quotient<ET>& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

void testCase(int p) {
    int a, b;
    std::cin >> a >> b;
    // set the coefficients of A and b
    const int X = 0;
    const int Y = 1;
    const int Z = 2;

    // std::cout << a << " " << b << "\n";

    Solution s;

    if (p == 1) {
        /*
        *   max b · y − a · x
        *      s.t. x, y >= 0
        *      x + y <= 4
        *      4x + 2y <= ab
        *      − x + y <= 1
        */
        // create an LP with Ax <= b, lower bound 0 and no upper bounds
        Program lp(CGAL::SMALLER, true, 0, false, 0);

        lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
        lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b); // 4x + 2y <= ab
        lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1

        // objective function: max by - ax => min -by + ax
        lp.set_c(X, a); // ax
        lp.set_c(Y, -b); // -by
        // lp.set_c0(0); // +64
        s = CGAL::solve_linear_program(lp, ET());
    }
    else {
        /*
        * min a · x + b · y + z
        *   s.t. x, y, z <= 0
        *   x + y >= −4
        *   4x + 2y + z >= −ab
        *   − x + y >= −1
        */

        // create an LP with Ax >= b, no lower bound and upper bound of 0
        Program lp(CGAL::LARGER, false, 0, true, 0);

        lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4); // x + y >= -4
        lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -(a * b)); // 4x + 2y + z >= -ab
        lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1); // -x + y >= -1

        // objective function: max by - ax => min -by + ax
        lp.set_c(X, a); // ax
        lp.set_c(Y, b); // by
        lp.set_c(Z, 1); // by
        // lp.set_c0(0); // +64
        s = CGAL::solve_linear_program(lp, ET());;
    }

    if (s.is_infeasible()) {
        std::cout << "no\n";
    }
    else if (s.is_unbounded()) {
        std::cout << "unbounded\n";
    }
    else {
        std::cout << (long)((p == 1) ? (-ceil_to_double(s.objective_value())) : ceil_to_double(s.objective_value())) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int p;
    std::cin >> p;
    do {
        testCase(p);
        std::cin >> p;
    } while (p != 0);
}