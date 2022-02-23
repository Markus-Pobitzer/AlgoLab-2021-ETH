#include <iostream>
#include <vector>
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
    int l, p;
    std::cin >> l >> p;

    std::vector<int> in(l);
    std::vector<int> out(l);
    long sum = 0;

    for (int i = 0; i < l; i++) {
        std::cin >> in[i] >> out[i];
    }

    std::vector<std::vector<int>> paths(p, std::vector<int>(3, 0));
    for (int i = 0; i < p; i++) {
        int f, t, c, C;
        std::cin >> f >> t >> c >> C;

        paths[i][0] = f;
        paths[i][1] = t;
        paths[i][2] = std::max(0, C - c);

        out[f] += c;
        in[t] += c;
    }

    graph G(l);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < l; i++) {
        adder.add_edge(v_source, i, in[i]);
        adder.add_edge(i, v_sink, out[i]);
        sum += out[i];
    }

    for (int i = 0; i < p; i++) {
        adder.add_edge(paths[i][0], paths[i][1], paths[i][2]);
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    if (flow == sum) {
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