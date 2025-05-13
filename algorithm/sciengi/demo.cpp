
/* Demo shows how to use edge-coloring algorithm *
 * on graph provided by adjacency matrix         */


#include <iostream>

#include <matrix/matrix.h>
#include <coloring/edge.h>



int main() {

    size_t side;
    std::cin >> side;

    matrix adj(side, side);
    std::cin >> adj;
 

    matrix colored_adj = color_edges(adj);


    std::cout << "\n'Colored' adjacency matrix:\n" << colored_adj << std::endl;;
}

