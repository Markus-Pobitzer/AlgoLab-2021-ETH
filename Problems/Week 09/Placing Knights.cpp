/*
* Compute the Minimum Vertex cover (MinVC) of th chessboard, i.e. remeber black and white squares
* Cimpute the Maximum independant set of the bipartite graph: MaxIS = IntactPlacements - MinVC
*
*/

#include <iostream>
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

void testCase() {
    int n;
    std::cin >> n;
    graph G(n * n);
    edge_adder adder(G);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int is; std::cin >> is;
            if (is) {
                count++;
                int ind = i * n + j;
                if ((i + j) % 2 == 1) {
                    adder.add_edge(ind, v_sink, 1);
                }
                else {
                    adder.add_edge(v_source, ind, 1);
                    if (i - 1 >= 0 && j - 2 >= 0) {
                        adder.add_edge(ind, (i - 1) * n + j - 2, 1);
                    }
                    if (i - 1 >= 0 && j + 2 < n) {
                        adder.add_edge(ind, (i - 1) * n + j + 2, 1);
                    }
                    if (i + 1 < n && j - 2 >= 0) {
                        adder.add_edge(ind, (i + 1) * n + j - 2, 1);
                    }
                    if (i + 1 < n && j + 2 < n) {
                        adder.add_edge(ind, (i + 1) * n + j + 2, 1);
                    }

                    if (i - 2 >= 0 && j - 1 >= 0) {
                        adder.add_edge(ind, (i - 2) * n + j - 1, 1);
                    }
                    if (i - 2 >= 0 && j + 1 < n) {
                        adder.add_edge(ind, (i - 2) * n + j + 1, 1);
                    }
                    if (i + 2 < n && j - 1 >= 0) {
                        adder.add_edge(ind, (i + 2) * n + j - 1, 1);
                    }
                    if (i + 2 < n && j + 1 < n) {
                        adder.add_edge(ind, (i + 2) * n + j + 1, 1);
                    }
                }
            }
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << (count - flow) << "\n";
}

int main() {
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
    return 0;
}