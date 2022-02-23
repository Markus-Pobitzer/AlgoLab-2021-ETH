///sample.in
#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


/*
  * To find the biggest radius we introduce a new variable in the LP, nemaly R.
  * The constraints are as described but in every constraint we use the scaled R,
  * i.e. norm * R to the left side. This reduces the avaialbe area corresponding
  * to R. The scale (i.e. norm) is needed otherwise we would not scale every side
  * the same.
*/
void testCase(int n, int d) {
    // create an LP with Ax <= b, lower bound 0 and no upper bounds
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    int R = d;
    for (int i = 0; i < n; i++) {
        long sqsum = 0;
        for (int j = 0; j < d; j++) {
            int in;
            std::cin >> in;
            lp.set_a(j, i, in);
            sqsum += std::pow(in, 2);
        }
        double norm = std::sqrt(sqsum);
        //std::cout << "norm: " << norm << "\n";
        lp.set_a(R, i, norm);
        int bi; std::cin >> bi;
        lp.set_b(i, bi);
    }
    lp.set_l(R, true, 0);
    lp.set_c(R, -1.0);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        std::cout << "none\n";
    }
    else {
        if (s.is_unbounded()) {
            std::cout << "inf\n";
        }
        else {
            long ret = -CGAL::to_double(s.objective_value());
            if (ret >= 0)
                std::cout << ret << "\n";
            else
                std::cout << "none\n";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, d;
    std::cin >> n;
    do {
        std::cin >> d;
        testCase(n, d);
        std::cin >> n;
    } while (n > 0);
}