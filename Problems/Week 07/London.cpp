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
    int h, w;
    std::cin >> h >> w;
    std::string word;
    std::cin >> word;
    const int abc = 'Z' - 'A' + 1;
    std::vector<int> outSink(abc, 0);
    std::vector<std::vector<int>> DP(abc, std::vector<int>(abc, 0));
    std::vector<std::vector<int>> fp(h, std::vector<int>(w));
    for (int i = 0; i < word.size(); i++) {
        outSink[word[i] - 'A'] += 1;
    }

    graph G(abc + 1);
    edge_adder adder(G);
    // Add special vertices source and sink
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    for (int i = 0; i < abc; i++) {
        if (outSink[i] > 0) {
            adder.add_edge(i, v_sink, outSink[i]);
            //std::cout << ((char) (i + 'A')) << ": " << outSink[i] << "\n";
        }
    }

    for (int i = 0; i < h; i++) {
        std::string line;
        std::cin >> line;
        //std::cout << (line[0] - 'A') << "\n";
        for (int j = 0; j < w; j++) {
            fp[i][j] = line[j] - 'A';
        }
    }

    for (int i = 0; i < h; i++) {
        std::string line;
        std::cin >> line;
        //std::cout << (line[0] - 'A') << "\n";
        for (int j = 0; j < w; j++) {
            DP[fp[i][j]][line[w - j - 1] - 'A'] += 1;
        }
    }

    for (int i = 0; i < abc; i++) {
        for (int j = 0; j < abc; j++) {
            if (DP[i][j] > 0) {
                vertex_desc local = boost::add_vertex(G);
                adder.add_edge(v_source, local, DP[i][j]);
                adder.add_edge(local, i, DP[i][j]);
                adder.add_edge(local, j, DP[i][j]);
            }
        }
    }

    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if (flow == word.size()) {
        std::cout << "Yes\n";
    }
    else {
        std::cout << "No\n";
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