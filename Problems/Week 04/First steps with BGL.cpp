#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


//typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::no_property, // no vertex property
    boost::property<boost::edge_weight_t, int> // edge property (interior)
> graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void testCase() {
    int n;
    int m;
    std::cin >> n;
    std::cin >> m;
    int sum = 0;
    int max = 0;

    graph G(n);
    for (int i = 0; i < m; i++) {
        int s, e, w;
        std::cin >> s;
        std::cin >> e;
        std::cin >> w;
        boost::add_edge(s, e, w, G);
    }

    std::vector<edge_desc> mst;

    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
        std::pair<edge_desc, bool> ed = boost::edge(boost::source(*it, G), boost::target(*it, G), G);
        sum += get(boost::edge_weight_t(), G, ed.first); // boost::source(*it, G), boost::target(*it, G)
        //std::cout << (*it)[0] << "\n";
    }
    /*
    BGL_FORALL_EDGES(e, G, graph)
    {
        //std::cout << source(e, g) << " -- " << target(e, g);
        if (std::find(mst.begin(), mst.end(), e) != mst.end())
            sum += get(edge_weight, g, e); //get the `weight` associated with `e`
    }
    */


    std::vector<int> dist_map(n); //exterior property
    boost::dijkstra_shortest_paths(G, 0, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    //return dist_map[t];
    for (int i = 1; i < n; i++) {
        if (max < dist_map[i]) {
            max = dist_map[i];
        }
    }

    std::cout << sum << " " << max << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}