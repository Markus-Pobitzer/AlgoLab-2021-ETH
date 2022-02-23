/*
* Min cost max flow (map to positive weights)
* Graph: from start to every buyer with (1, 0), i.e. flow = 1 (only one estate intereted) and 0 cost
*        From ever buyer to every estate with (1, -(bid - 100)), i.e. the bid cost - 100 and than change ign to +
*        From every estate to the state where the estate is in (1, 0)
*        From every State to Sink with (S1, 0)
*        Output flow and winn = flow * 100 - cost
*/


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

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor,
    boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
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
    int N, M, S;
    std::cin >> N >> M >> S;
    std::vector<int> states(S);
    for (int i = 0; i < S; i++) {
        std::cin >> states[i];
    }
    std::vector<int> inState(M);
    for (int i = 0; i < M; i++) {
        std::cin >> inState[i];
    }
    std::vector<std::vector<int>> bids(N, std::vector<int>(M));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int b;
            std::cin >> b;
            // bids[i][j] = -b;
            bids[i][j] = -(b - 100);
        }
    }
    graph G(N + M + S);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < N; i++) {
        adder.add_edge(v_source, i, 1, 0);
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            adder.add_edge(i, N + j, 1, bids[i][j]);
            //std::cout << bids[i][j] << " ";
        }
        //std::cout << "\n";
    }

    for (int i = 0; i < M; i++) {
        adder.add_edge(N + i, N + M + inState[i] - 1, 1, 0);
        //std::cout << inState[i] << "\n";
    }

    for (int i = 0; i < S; i++) {
        adder.add_edge(N + M + i, v_sink, states[i], 0);
    }

    int flow1 = boost::push_relabel_max_flow(G, v_source, v_sink);
    //boost::cycle_canceling(G);
    //int cost1 = boost::find_flow_cost(G);
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost2 = boost::find_flow_cost(G);

    std::cout << flow1 << " " << (flow1 * 100 - cost2) << "\n";
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
