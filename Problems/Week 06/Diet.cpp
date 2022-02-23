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

void testCase(int n, int m) {
    std::vector<std::pair<int, int>> amount(n);
    std::vector<int> prices(m);
    std::vector<std::vector<int>> nutri(m, std::vector<int>(n));

    //std::cout << n << " " << m << "\n";
    for (int i = 0; i < n; i++) {
        int min, max;
        std::cin >> min >> max;
        amount[i] = std::make_pair(min, max);
    }

    for (int i = 0; i < m; i++) {
        int pi;
        std::cin >> pi;
        prices[i] = pi;
        //std::cout << pi <<" ";
        for (int j = 0; j < n; j++) {
            int Cij;
            std::cin >> Cij;
            nutri[i][j] = Cij;
        }
    }

    //std::cout << "\nhHERE\n";
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    int fcounter = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            lp.set_a(j, fcounter, -nutri[j][i]);
            lp.set_a(j, fcounter + 1, nutri[j][i]);
        }
        lp.set_b(fcounter, -amount[i].first);
        lp.set_b(fcounter + 1, amount[i].second);
        fcounter += 2;
    }

    for (int j = 0; j < m; j++) {
        lp.set_c(j, prices[j]);
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        std::cout << "No such diet.\n";
    }
    else {
        std::cout << (long)floor_to_double(s.objective_value()) << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, m;
    std::cin >> n >> m;
    do {
        testCase(n, m);
        std::cin >> n >> m;
    } while (n != 0 && m != 0);
}