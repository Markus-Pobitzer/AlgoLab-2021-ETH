
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <algorithm>
#include <boost/config.hpp>

namespace boost
{
    struct edge_component_t
    {
        enum
        {
            num = 555
        };
        typedef edge_property_tag kind;
    } edge_component;
}

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
    boost::property< boost::edge_component_t, std::size_t > > graphG;
typedef boost::graph_traits<graphG>::edge_iterator edge_it;
typedef boost::graph_traits<graphG>::vertex_descriptor vertex_it;

/*
  =========== Sol ================
  We compute the biconnected component of every edge in the graph.
  If a Biconected component has only one edge, we know it is a bridge.
  I.e. go over all edges, look in which component it lies and sum up the component number
  If we see that the sum of component of an edge is == 1 we have a bridge.
*/

void testCase() {
    using namespace boost;
    int n, m;
    std::cin >> n >> m;
    graphG G(n);
    for (int i = 0; i < m; i++) {
        int e1, e2;
        std::cin >> e1 >> e2;
        boost::add_edge(e1, e2, G);
        //std::cout << e1 << " --- " << e2 << "\n";
    }
    //std::cout << "\n";

    edge_it ebeg, eend;

    std::vector<std::pair<int, int>> ret(0);
    boost::property_map <graphG, edge_component_t >::type component = get(edge_component, G);

    std::size_t num_comps = biconnected_components(G, component);


    std::vector<int> compNumber(num_comps);

    graph_traits< graphG >::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        compNumber[component[*ei]] += 1;
        // std::cout << source(*ei, G) << " -- " << target(*ei, G) << "[label=\"" << component[*ei] << "\"]\n";
    }

    for (boost::tie(ei, ei_end) = edges(G); ei != ei_end; ++ei) {
        if (compNumber[component[*ei]] == 1) {
            ret.push_back(std::make_pair(source(*ei, G), target(*ei, G)));
        }
    }

    for (long unsigned int i = 0; i < ret.size(); i++) {
        int f = ret[i].first;
        int s = ret[i].second;
        if (f > s) {
            int swap = f;
            f = s;
            s = swap;
        }
        ret[i] = std::make_pair(f, s);
    }
    std::sort(ret.begin(), ret.end());
    std::cout << ret.size() << "\n";
    for (long unsigned int i = 0; i < ret.size(); i++) {
        std::cout << ret[i].first << " " << ret[i].second << "\n";
    }


}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}