#include <iostream>
#include <vector>

/*
* Solved with flow. Doulbe every team for the nrormal games (m) and difficult ones (d).
* For e and w create a new vertex. FreeE = p - e * l and FreeW = p - w * l. That stores
* the actual flows that are still free. Than go from e to w with every entry and
* also see that all e´s have an l input and w an  output.
*
*/

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

void testCase(int t) {
    int e, w, m, d, p, l;
    std::cin >> e >> w >> m >> d >> p >> l;

    graph G(2 * (e + w) + 4);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const int v_source = 2 * (e + w);
    const int v_freeE = v_source + 1;
    const int v_freeW = v_source + 2;
    const int v_target = v_source + 3;

    const int freeE = p - e * l;
    const int freeW = p - w * l;

    adder.add_edge(v_source, v_freeE, freeE, 0);
    adder.add_edge(v_freeW, v_target, freeW, 0);
    for (int i = 0; i < e; i++) {
        adder.add_edge(v_source, 2 * i, l, 0);
        adder.add_edge(v_freeE, 2 * i, freeE, 0);
        adder.add_edge(v_freeE, 2 * i + 1, freeE, 0);
    }

    for (int i = 0; i < w; i++) {
        adder.add_edge(2 * (e + i), v_target, l, 0);
        adder.add_edge(2 * (e + i), v_freeW, freeW, 0);
        adder.add_edge(2 * (e + i) + 1, v_freeW, freeW, 0);
    }

    for (int i = 0; i < m; i++) {
        int u, v, r;
        std::cin >> u >> v >> r;

        adder.add_edge(2 * u, 2 * (e + v), 1, r);
    }


    for (int i = 0; i < d; i++) {
        int u, v, r;
        std::cin >> u >> v >> r;

        adder.add_edge(2 * u + 1, 2 * (e + v) + 1, 1, r);
    }

    if (freeE >= 0 && freeW >= 0) {
        // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
        int cost2 = boost::find_flow_cost(G);

        int s_flow = 0;
        out_edge_it eit, eend;
        for (boost::tie(eit, eend) = boost::out_edges(boost::vertex(v_source, G), G); eit != eend; ++eit) {
            s_flow += c_map[*eit] - rc_map[*eit];
        }
        // std::cout << "Flow : " << s_flow << " cost: " << cost2 << "\n";
        if (s_flow == p) {
            std::cout << cost2 << "\n";
        }
        else {
            std::cout << "No schedule!\n";
        }
    }
    else {
        std::cout << "No schedule!\n";
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase(i);
    }
}