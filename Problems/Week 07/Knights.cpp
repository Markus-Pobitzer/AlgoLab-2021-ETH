///1
#include <iostream>
#include <vector>
#include <algorithm>

// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
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

void testCase() {
    int m, n, k, c;
    std::cin >> m >> n >> k >> c;
    graph G(2 * m * n);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    std::vector<int> knights(k);

    for (int i = 0; i < k; i++) {
        int a, b;
        std::cin >> a >> b;
        knights[i] = a + b * m;
    }

    std::sort(knights.begin(), knights.end());

    int ind = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int out = i * m + j; // i * m + j is index of out vertes
            int in = i * m + j + n * m; // i * m + j + 2nm is index of in edge

            // The capacity of the actual node split up on two nodes
            adder.add_edge(in, out, c);

            if (ind < k && i * m + j == knights[ind]) {
                adder.add_edge(v_source, in, 1);
                ind++;
            }
            if (i != 0) {
                adder.add_edge(out - m, in, 1);
                adder.add_edge(out, in - m, 1);
            }
            if (j != 0) {
                adder.add_edge(out - 1, in, 1);
                adder.add_edge(out, in - 1, 1);
            }
            if ((i == 0 || i == n - 1) && (j == 0 || j == m - 1)) {
                adder.add_edge(out, v_sink, 2);
            }
            else if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                adder.add_edge(out, v_sink, 1);
            }
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << flow << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}