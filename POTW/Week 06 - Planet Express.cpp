#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

/*
  Set up the graph but invert all th directions, s.t. we can get Dikstra from
  End node to every warehouse in one go.
  To see which teporters are actually up we use strong components
  ( <==> paiwise reachable). Now for evey strong component count
  how many teleporters are in (weight = #teleporters -1). If we have
  some, add a vertex for every such component and edges from the associated
  teleportation networks and back once with 0 cost and the other with the count.
  Now use Dijkstra.

  result must be <= 1000000
*/

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::no_property, // no vertex property
    boost::property<boost::edge_weight_t, int> // edge property (interior)
> weighted_graph;


void testCase() {
    int n, m, k, T;
    std::cin >> n >> m >> k >> T;

    std::vector<int> network(T);
    for (int i = 0; i < T; i++) {
        std::cin >> network[i];
    }

    weighted_graph G(n);
    //std::vector<std::vector<int>> edges(n);
    for (int i = 0; i < m; i++) {
        int u, v, c;
        std::cin >> u >> v >> c;
        boost::add_edge(v, u, c, G);
    }

    std::vector<int> scc_map(n); // exterior property map
    int nscc = boost::strong_components(G, boost::make_iterator_property_map(scc_map.begin(), boost::get(boost::vertex_index, G)));

    std::vector<int> count(nscc, -1);
    for (int i = 0; i < T; i++) {
        count[scc_map[network[i]]] += 1;
    }
    for (int i = 0; i < T; i++) {
        // For every strong connected compoent add an extra vertex
        // Every teleportation network in this component gets a connection to this
        // vertex with once 0 cost and cost of count of the amount of teleporter
        boost::add_edge(n + scc_map[network[i]], network[i], count[scc_map[network[i]]], G);
        boost::add_edge(network[i], n + scc_map[network[i]], 0, G);
    }

    std::vector<int> dist_map(boost::num_vertices(G)); //exterior property
    boost::dijkstra_shortest_paths(G, n - 1, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));

    int min = 1000001;
    for (int i = 0; i < k; i++) {
        // std::cout << i << ": " << dist_map[i] << "\n";
        min = std::min(min, dist_map[i]);
    }

    if (min > 1000000) {
        std::cout << "no\n";
    }
    else {
        std::cout << min << "\n";
    }
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