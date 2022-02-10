// example: decide whether two point sets R and B can be separated by a nonvertical line
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>


/*
  LP. For the sewage we have a line ax + by + c = 0. Since we want it to be non
  horizontally we can set a = 1 --> x + by + c = 0.

  1) For the first problem we need to check that all nobles (px, py) ar on the
  left of the line, px + b * py <= c --> b * py - c <= -px.
  And for the commoners (px, py) we check if they are on the right side of the
  line px + b * py >= c --> -b * py + c <= px.
  ---> If infeasable: "Yuck!"

  2) Notice that we are only interested in the length of the x-Axis. So a house
  (px, py) has x-value x on the line x = -b * py - c. Since the nobles will have
  px < x and the commoners have x < px we can write the sum of all the x-lengths
  as -b * py - c - px (noble) + px + b * py + c (commoner) <= s
  and the whole sum is: all noble (-b * py) + all commoner(b * py) +
  + (m - n) * c <= s + n * px - m * px
  ---> If infeasable: "Bankrupt!"

  3) The line that is perependicualr to the sewage canal is bx -y + d = 0. Where
  d is a new variable. And now we are inerested in the y-length of the line. For
  a point (px, py) we get the y-length to the line as y = b * px + d. And we
  create a new variable e that tells us the largest length of such a line.
  ¦y - py¦ <= e. And for py < y we have b * px + d - e <= py and for y >= py we
  have -b * px - d - e <= -py. And this are our last constraints. We minimize
  over e and set a lower bound e >= 0. Output ceild e to long.



*/

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


long ceil_to_long(const CGAL::Quotient<ET>& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}


void testCase() {
    // create an LP with Ax <= b and no lower/upper bounds
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    const int b = 0;
    const int c = 1;

    // number of red and blue points
    int n; std::cin >> n;
    int m; std::cin >> m;
    long s; std::cin >> s;
    int counter = 0;

    std::vector<std::vector<int>> noble(n, std::vector<int>(2));
    std::vector<std::vector<int>> comm(m, std::vector<int>(2));
    // read the blue points
    for (int i = 0; i < n; ++i) {
        int x; std::cin >> x;
        int y; std::cin >> y;
        // set up constraint x + b y + c >= 0
        // set up constraint - b y - c <= x
        lp.set_a(b, counter, y);
        lp.set_a(c, counter, 1);
        lp.set_b(counter, -x);
        noble[i] = { x, y };
        counter++;
    }

    // read the red points
    for (int i = 0; i < m; ++i) {
        int x; std::cin >> x;
        int y; std::cin >> y;
        // set up constraint b y + c <= - x
        lp.set_a(b, counter, -y);
        lp.set_a(c, counter, -1);
        lp.set_b(counter, x);
        comm[i] = { x, y };
        counter++;
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) {
        std::cout << "Yuck!" << "\n";
        return;
    }

    if (s != -1) {
        long sny = 0;
        long snx = 0;
        long scy = 0;
        long scx = 0;
        for (int i = 0; i < n; i++) {
            snx += noble[i][0];
            sny -= noble[i][1];
        }
        for (int i = 0; i < m; i++) {
            scy += comm[i][1];
            scx -= comm[i][0];
        }
        lp.set_a(b, counter, sny + scy);
        lp.set_a(c, counter, m - n);
        lp.set_b(counter, s + snx + scx);
        counter++;
        sol = CGAL::solve_linear_program(lp, ET());
        if (sol.is_infeasible()) {
            std::cout << "Bankrupt!" << "\n";
            return;
        }
    }

    const int d = 2;
    const int e = 3;

    for (int i = 0; i < n; i++) {
        lp.set_a(b, counter, noble[i][0]);
        lp.set_a(d, counter, 1);
        lp.set_a(e, counter, -1);
        lp.set_b(counter, noble[i][1]);
        counter++;

        lp.set_a(b, counter, -noble[i][0]);
        lp.set_a(d, counter, -1);
        lp.set_a(e, counter, -1);
        lp.set_b(counter, -noble[i][1]);
        counter++;
    }

    for (int i = 0; i < m; i++) {
        lp.set_a(b, counter, comm[i][0]);
        lp.set_a(d, counter, 1);
        lp.set_a(e, counter, -1);
        lp.set_b(counter, comm[i][1]);
        counter++;

        lp.set_a(b, counter, -comm[i][0]);
        lp.set_a(d, counter, -1);
        lp.set_a(e, counter, -1);
        lp.set_b(counter, -comm[i][1]);
        counter++;
    }

    lp.set_c(e, 1);
    lp.set_l(e, true, 0);
    sol = CGAL::solve_linear_program(lp, ET());
    std::cout << ceil_to_long(sol.objective_value()) << "\n";
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        testCase();
    }
}
