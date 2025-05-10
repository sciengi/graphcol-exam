#include <utils/general.h>



size_t find_degree_of_graph(matrix& adj) {

    size_t max_degree = 0, current_degree;
    for (size_t i = 0; i < adj.get_row_count(); i++) {

        current_degree = 0;
        for (size_t j = 0; j < adj.get_row_count(); j++) {
            if (adj[i][j] == 0) continue;
            current_degree++;
        }

        if (current_degree > max_degree)
            max_degree = current_degree;
    }

    return max_degree;
}
