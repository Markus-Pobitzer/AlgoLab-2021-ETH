#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor,
    boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator


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


void testCase(int t) {
    int tester = -1;
    int c, g, k, a;
    long b;
    std::cin >> c >> g >> b >> k >> a;
    const int source = c + 1;

    graph G(c + 1);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    int inflow = 0;
    for (int i = 0; i < g; i++) {
        int x, y, d, e;
        std::cin >> x >> y >> d >> e;
        adder.add_edge(x, y, e, d);
        if (x == k) {
            inflow += e;
        }
    }
    adder.add_edge(source, k, inflow, 0);

    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    long cost2 = boost::find_flow_cost(G);
    int mF = 0;
    out_edge_it e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e) {
        mF += c_map[*e] - rc_map[*e];
    }
    int ret = mF;
    if (t == tester) {
        std::cout << "Cost: " << cost2 << " flow: " << ret << "\n";
    }
    if (cost2 > b) {
        edge_desc ed = boost::edge(source, k, G).first;
        int low = 0;
        int up = ret;
        bool bo = true;
        while (low < up && bo) {
            int add = (up - low) / 2;
            if (add == 0) {
                bo = false;
            }
            int mid = low + add;
            c_map[ed] = mid;

            // std::cout << low << " to " << up << " " << mid << std::endl;
            boost::successive_shortest_path_nonnegative_weights(G, source, a);
            cost2 = boost::find_flow_cost(G);
            if (cost2 > b) {
                up = mid;
            }
            else {
                low = mid;
                ret = low;
            }
        }
    }

    std::cout << ret << "\n";
}


int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
    return 0;
}