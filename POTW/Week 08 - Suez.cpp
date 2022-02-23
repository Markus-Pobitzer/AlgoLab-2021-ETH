///1
#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

/*
  LP. Observe that size limitation of every two posters can be represented in one
  linear equation:
  ai + aj <= 2 * max(abs(xi - xj) / w, abs(yi - yj) / h)
  or in the case where there is an old poster we want the minimum of all where one:
  ai  <= 2 * max(abs(xi - xj) / w, abs(yi - yj) / h) - 1
  Using these constraints, with all new posters x all new posters and all new
  posters x min(max.. old poster) we get the final rsult.

*/

long ceil_to_double(const CGAL::Quotient<CGAL::Gmpq>& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a >= x + 1) a -= 1;
    return a;
}

void testCase() {
    int n, m, h, w;
    std::cin >> n >> m >> h >> w;

    std::vector<std::vector<int>> newP(n, std::vector<int>(2));
    std::vector<std::vector<int>> oldP(m, std::vector<int>(2));

    for (int i = 0; i < n; i++) {
        std::cin >> newP[i][0] >> newP[i][1];
    }

    for (int i = 0; i < m; i++) {
        std::cin >> oldP[i][0] >> oldP[i][1];
    }

    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp(CGAL::SMALLER, true, 1, false, 0);

    int counter = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            IT e1 = IT(std::abs(newP[i][0] - newP[j][0])) / w;
            IT e2 = IT(std::abs(newP[i][1] - newP[j][1])) / h;
            // ai + aj <= 2 * max(e1, e2)
            lp.set_a(i, counter, 1); lp.set_a(j, counter, 1); lp.set_b(counter, 2 * std::max(e1, e2));
            counter++;
        }
        IT min = IT(-1);
        for (int j = 0; j < m; j++) {
            IT e1 = IT(std::abs(newP[i][0] - oldP[j][0])) / w;
            IT e2 = IT(std::abs(newP[i][1] - oldP[j][1])) / h;

            if (min == -1) {
                min = std::max(e1, e2);
            }
            else {
                min = std::min(min, std::max(e1, e2));
            }
        }
        if (min != -1) {
            // ai <= 2 * min - 1
            lp.set_a(i, counter, 1); lp.set_b(counter, 2 * min - 1);
            counter++;
        }

        lp.set_c(i, IT(-2 * w - 2 * h));
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    std::cout << ceil_to_double(-s.objective_value()) << "\n";
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