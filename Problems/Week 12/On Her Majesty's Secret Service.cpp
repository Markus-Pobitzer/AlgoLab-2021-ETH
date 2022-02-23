/*
  ONLY 50%!
*/

#include <iostream>
#include <vector>
#include <boost/graph/dijkstra_shortest_paths.hpp>
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
typedef traits::vertex_descriptor vertex_desc;

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property, // no vertex property
    boost::property<boost::edge_weight_t, int> // edge property (interior)
> weighted_graph;

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
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;

    weighted_graph G(n);
    for (int i = 0; i < m; i++) {
        char type;
        int x, y, z;
        std::cin >> type >> x >> y >> z;

        boost::add_edge(x, y, z, G);
        if (type == 'L') {
            boost::add_edge(y, x, z, G);
        }
    }

    std::vector<int> local_1(n, 0);
    std::vector<int> agents;
    std::vector<int> count_agents;

    std::vector<int> local_2(n, 0);
    std::vector<int> shelters;
    std::vector<int> count_shelters;

    for (int i = 0; i < a; i++) {
        int index;
        std::cin >> index;
        local_1[index] += 1;
        // std::cin >> agents[i];
    }
    for (int i = 0; i < n; i++) {
        if (local_1[i] > 0) {
            agents.push_back(i);
            count_agents.push_back(local_1[i]);
        }
    }

    for (int i = 0; i < s; i++) {
        int index;
        std::cin >> index;
        local_2[index] += 1;
        //std::cin >> shelters[i];
    }

    for (int i = 0; i < n; i++) {
        if (local_2[i] > 0) {
            shelters.push_back(i);
            count_shelters.push_back(local_2[i]);
        }
    }

    a = (int)agents.size();
    s = (int)shelters.size();
    std::vector<std::vector<long>> costs(a, std::vector<long>(s));
    for (int i = 0; i < a; i++) {
        std::vector<int> dist_map(n); //exterior property
        boost::dijkstra_shortest_paths(G, agents[i],
            boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
                boost::get(boost::vertex_index, G))));
        for (int j = 0; j < s; j++) {
            costs[i][j] = dist_map[shelters[j]];
            // std::cout << costs[i][j] << " ";
        }
        //std::cout << "\n";
    }

    graph FG(2 * n);
    edge_adder adder(FG);
    const vertex_desc v_source = boost::add_vertex(FG);
    const vertex_desc v_sink = boost::add_vertex(FG);
    auto c_map = boost::get(boost::edge_capacity, FG);
    auto r_map = boost::get(boost::edge_reverse, FG);
    auto rc_map = boost::get(boost::edge_residual_capacity, FG);


    for (int i = 0; i < a; i++) {
        for (int j = 0; j < s; j++) {
            if (costs[i][j] < 2147483647) {
                long lc = costs[i][j];
                adder.add_edge(agents[i], n + shelters[j], 1, lc * lc);
            }
        }
        adder.add_edge(v_source, agents[i], count_agents[i], 0);
    }
    for (int j = 0; j < s; j++) {
        adder.add_edge(n + shelters[j], v_sink, count_shelters[j], 0);
    }

    boost::successive_shortest_path_nonnegative_weights(FG, v_source, v_sink);
    // boost::find_flow_cost(FG);

    if (t == -1) {
        std::cout << a << "\n";
        int s_flow = 0;
        out_edge_it e, eend;
        for (boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source, FG), FG); e != eend; ++e) {
            std::cout << "edge from " << boost::source(*e, FG) << " to " << boost::target(*e, FG)
                << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
            s_flow += c_map[*e] - rc_map[*e];
        }
        std::cout << "s-out flow " << s_flow << "\n"; // 5
    }

    long max = 0;
    for (int i = 0; i < a; i++) {
        /*
        out_edge_it e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,FG), FG); e != eend; ++e) {
            //std::cout << "edge from " << boost::source(*e, FG) << " to " << boost::target(*e, FG)
            //    << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
            // s_flow += c_map[*e] - rc_map[*e];
            max = std::max(max, (c_map[*e] - rc_map[*e]) * costs[i][boost::target(*e, FG) - n]);
        }*/
        for (int j = 0; j < s; j++) {
            if (costs[i][j] < 2147483647) {
                int ag = agents[i];
                int sh = shelters[j] + n;
                // std::cout << i << ": " << ag << " " << j << ": " << sh << std::endl;
                edge_desc edge = boost::edge(ag, sh, FG).first;
                // std::cout << edge << std::endl;
                max = std::max(max, (c_map[edge] - rc_map[edge]) * costs[i][j]);
                // std::cout << (c_map[edge] - rc_map[edge]) << std::endl;
            }
        }
    }

    std::cout << (max + d) << "\n";

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