///1
#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_capacity_t, long, boost::property<boost::edge_residual_capacity_t, long, boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
    graph& G;

public:
    explicit edge_adder(graph& G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};


/*
  * Just normal max flow, add edge to both directions. Connect each vertex with at least
  * one store with the sink. Capacity of the edge is the number of stores on this vertex.
*/
void testCase() {
    int n, m, s;
    std::cin >> n >> m >> s;
    graph G(n);
    edge_adder adder(G);

    std::vector<int> stores(n, 0);
    for (int i = 0; i < s; i++) {
        int pos;
        std::cin >> pos;
        stores[pos] += 1;
    }

    for (int i = 0; i < m; i++) {
        int st, et;
        std::cin >> st >> et;
        adder.add_edge(st, et, 1);
        adder.add_edge(et, st, 1);
    }

    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    adder.add_edge(v_source, 0, s);
    for (int i = 0; i < n; i++) {
        if (stores[i] > 0) {
            adder.add_edge(i, v_sink, stores[i]);
        }
    }

    // Calculate flow from source to sink
    // The flow algorithm uses the interior properties (managed in the edge adder)
    // - edge_capacity, edge_reverse (read access),
    // - edge_residual_capacity (read and write access).
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    //std::cout << flow << "\n";

    if (flow == (long)s) {
        std::cout << "yes\n";
    }
    else {
        std::cout << "no\n";
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}