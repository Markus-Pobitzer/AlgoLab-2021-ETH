// Includes
// ========
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <vector>
#include <map>
#include <algorithm>


/*
  Flow problem. Every relevant timestamp gets S vertices in the graph. Relevant is
  0 and every other timestamp where a rental job starts / ends (di, ai). Give each
  such timestamp a unique ID and let the Graph have (S * #timestaps + 2) vertices.
  Also store the relevant timestamp for each rental station and sort them.

  Link the source to every rental station with timestamp = 0, flow = li. Connect
  always the next relevant timestamp with the current timestamp, set flow to max
  number of cars (i.e. sum of all li) and the cost of the flow to
  100 * (t_{j + 1} - t_j), where t_{j + 1} >= t_j. I.e. the cost is how much we
  would lose if we let the cars stand idle at the station. Connect also the last
  vertex to t_sink with flow = sum cars, and cost = 0.

  For all the jobs, connect the corresponding departure station at the right
  timestamp with the corresponding arrival station at the right timestamp. Set
  flow = 0, and cost = 100 * (ai - di) - pi.

  The output is than: sumCars * maxTimestamp * 100 - flowCost.

  Why the costs?
  We know that the max price pi is 100. To make a Min cost algorithm we say the
  actual cost is 100 - pi. So all the cots are still positive (faster algo.)
  and minimizing this cost is the same as maximizing the profit. The next step
  is to get the actual cost. We say every traversal between a time step costs 100
  so if a car stays idle for one time it costs us 100 otherwise. If a job connects
  two stations with time di and ai the actual cost is
  100 * (ai - di - 1) + 100 - pi = 100 * (ai - di) - pi.


             --> (1) ---> ... ---> (1) --
  v_source ----> (2) ---> ... ---> (2) ---> v_sink
             --> (3) ---> ... ---> (3) --

  timesamp:       0                max
*/


// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor,
    boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
    graph& G;

public:
    explicit edge_adder(graph& G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;   // new assign cost
        w_map[rev_e] = -cost;   // new negative cost
    }
};

void testCase() {
    int N, S;
    std::cin >> N >> S;
    std::map<int, int> map;
    std::vector<int> isIn(100001, 0);
    int max = -1;


    int sumCars = 0;
    std::vector<int> station(S);
    for (int i = 0; i < S; i++) {
        int li;
        std::cin >> li;
        station[i] = li;
        sumCars += li;
    }

    std::vector<std::vector<int>> jobs(N, std::vector<int>(5));
    std::vector<std::vector<int>> eToS(S);
    for (int i = 0; i < N; i++) {
        int si, ti, di, ai, pi;
        std::cin >> si >> ti >> di >> ai >> pi;
        si--;
        ti--;
        isIn[di] = 1;
        isIn[ai] = 1;
        eToS[si].push_back(di);
        eToS[ti].push_back(ai);
        max = std::max(max, std::max(di, ai));
        jobs[i] = { si, ti, di, ai, pi };
    }

    isIn[0] = 1;
    for (int i = 0; i < S; i++) {
        eToS[i].push_back(0);
        eToS[i].push_back(max);
        std::sort(eToS[i].begin(), eToS[i].end());
    }

    int index = 0;
    for (int i = 0; i < 100001; i++) {
        if (isIn[i]) {
            map.insert(std::make_pair(i, index));
            index++;
        }
    }

    int mul = index;

    graph G(S * mul + 2);
    const int v_source = S * mul;
    const int v_target = v_source + 1;
    edge_adder adder(G);

    for (int i = 0; i < S; i++) {
        adder.add_edge(v_source, S * map.at(eToS[i][0]) + i, station[i], eToS[i][0] * 100);
    }

    for (int i = 0; i < N; i++) {
        int dep = map.at(jobs[i][2]);
        int arr = map.at(jobs[i][3]);

        adder.add_edge(dep * S + jobs[i][0], arr * S + jobs[i][1], 1, (jobs[i][3] - jobs[i][2]) * 100 - jobs[i][4]);
    }

    for (int i = 0; i < S; i++) {
        for (int j = 0; j < (int)eToS[i].size() - 1; j++) {
            adder.add_edge(S * map.at(eToS[i][j]) + i, S * map.at(eToS[i][j + 1]) + i, sumCars, (eToS[i][j + 1] - eToS[i][j]) * 100);
        }
    }

    for (int j = 0; j < S; j++) {
        adder.add_edge(S * (mul - 1) + j, v_target, sumCars, 0);
    }

    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    long cost2 = boost::find_flow_cost(G);
    std::cout << ((long)sumCars * 100 * (max)-cost2) << "\n";
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
