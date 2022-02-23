
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    boost::no_property, // no vertex property
    boost::property<boost::edge_weight_t, int> // edge property (interior)
> weighted_graph;

typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

int dijkstra_dist(const weighted_graph& G, int s, int t) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n); //exterior property
    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
            boost::get(boost::vertex_index, G))));
    return dist_map[t];
}

// We create an MST for every species in the forst, Extract the min edges out oft all the msts,
// i.e. one edge gets assigned a value form a species network. From this new graph take the dijkstra from a to b.
// Ignore the hive number, not relevant
void testCase() {
    int n, e, s, a, b;
    std::cin >> n >> e >> s >> a >> b;

    std::vector<std::vector<std::vector<int>>> sToEdge(s, std::vector<std::vector<int>>(e, std::vector<int>(3)));
    std::map<std::pair<int, int>, int> bestEdge;

    for (int i = 0; i < e; i++) {
        int t1, t2;
        std::cin >> t1 >> t2;
        for (int j = 0; j < s; j++) {
            int wj;
            std::cin >> wj;
            sToEdge[j][i][0] = wj;
            sToEdge[j][i][1] = t1;
            sToEdge[j][i][2] = t2;
        }
    }

    for (int j = 0; j < s; j++) {
        int hj;
        std::cin >> hj;
    }

    for (int i = 0; i < s; i++) {
        weighted_graph G(n);
        for (int j = 0; j < e; j++) { // MST ?!
            int t1 = sToEdge[i][j][1];
            int t2 = sToEdge[i][j][2];
            int cv = sToEdge[i][j][0];
            boost::add_edge(t1, t2, cv, G);
        }

        std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
        for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
            int t1, t2, w;
            t1 = boost::source(*it, G);
            t2 = boost::target(*it, G);

            std::pair<edge_desc, bool> ed = boost::edge(t1, t2, G);
            w = get(boost::edge_weight_t(), G, ed.first);

            std::pair<int, int> pa(t1, t2);
            if (bestEdge.count(pa) == 0) {
                bestEdge.insert(std::make_pair(pa, w));
            }
            else if (bestEdge.at(pa) > w) {
                bestEdge.at(pa) = w;
            }
        }
    }

    weighted_graph G(n);
    std::map<std::pair<int, int>, int>::iterator it;
    for (it = bestEdge.begin(); it != bestEdge.end(); it++)
    {
        boost::add_edge((it->first).first, (it->first).second, it->second, G);
        //std::cout << (it->first).first << " " << (it->first).second << " " << it->second << "\n";
    }
    std::cout << dijkstra_dist(G, a, b) << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}