#include <iostream>
#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>


typedef boost::adjacency_list<boost::vecS,
    boost::vecS,
    boost::undirectedS> graph;

typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

/*
  * For every student (i.e. vertex in the graph) pair compute how many interests
  * they have in common. If they have more interests in common than f (the optimal)
  * number of matching, we create a undirected edge between them.
  * Afterwards compute the biggest cardinality matching (i.e. one vertex can only
  * have one of its edges in the matching) and see if it is exctly n/2, i.e. there
  * exists a better matching as the propsed one
  *
*/
void testCase() {
    int n, c, f;
    std::cin >> n >> c >> f;
    std::vector<std::vector<int>> pplHobb(n, std::vector<int>(c, 0));
    std::map<std::string, int> charToInt;
    graph G(n);

    int pos = 0;
    for (int i = 0; i < n; i++) {
        std::string charac;
        for (int j = 0; j < c; j++) {
            std::cin >> charac;
            if (charToInt.count(charac) == 0) {
                pplHobb[i][j] = pos;
                charToInt.insert(std::make_pair(charac, pos));
                pos++;
            }
            else {
                pplHobb[i][j] = charToInt.at(charac);
            }
        }
    }

    std::vector<bool> isC(pos, false);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = 0; k < c; k++) {
                isC[pplHobb[i][k]] = true;
                //std::cout << i << ": true at " << pplHobb[i][k] << "\n";
            }
            int count = 0;
            for (int k = 0; k < c; k++) {
                //std::cout << j << ": true at " << pplHobb[j][k] << "\n";
                if (isC[pplHobb[j][k]]) {
                    count++;
                }
            }
            if (count > f) {
                boost::add_edge(i, j, G);
                //std::cout << i << " -- " << j << "\n";
            }

            for (int k = 0; k < c; k++) {
                isC[pplHobb[i][k]] = false;
            }
        }
    }

    std::vector<vertex_desc> mate_map(n); // exterior property map

    boost::edmonds_maximum_cardinality_matching(G,
        boost::make_iterator_property_map(mate_map.begin(),
            boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,
        boost::make_iterator_property_map(mate_map.begin(),
            boost::get(boost::vertex_index, G)));
    //std::cout << matching_size << "\n";
    if (matching_size == (n / 2)) {
        std::cout << "not optimal\n";
    }
    else {
        std::cout << "optimal\n";
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for (int i = 0; i < t; i++) {
        testCase();
    }
}