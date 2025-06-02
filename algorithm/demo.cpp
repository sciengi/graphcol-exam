
/* Demo shows how to use edge-coloring algorithm *
 * on graph provided by adjacency matrix         */


#include <cassert>
#include <iostream>

#include <matrix/matrix.h>
#include <coloring/edge_tesing.h>
#include <utils/general.h>



int main() {

    std::cout 
        << "Please, enter graph as a row count and adjacency matrix:\n" 
        << "(Use 1 for connections and 0 for not)" << std::endl;

    int side;
    std::cin >> side;
    if (std::cin.fail() || side <= 0) {
        std::cout << "Failed to recieve data" << std::endl;
        return 0;
    }

    matrix adj(side, side);
    std::cin >> adj;
 

    matrix colored_adj = color_edges(adj);


    std::cout 
        << "\n'Colored' adjacency matrix:\n" << colored_adj << std::endl;

    size_t degree       = find_degree_of_graph(adj);
    size_t colors_count = find_unique_colors(colored_adj, 0).size();
    std::cout 
        << "Using " << colors_count  << " colors, "
        << "when degree of graph is " << degree << std::endl;
    assert(colors_count <= degree + 1);


    bool is_correct = edge_coloring_is_correct(colored_adj, 0);
    assert(is_correct);
    std::cout << "Coloring is correct" << std::endl;
}

