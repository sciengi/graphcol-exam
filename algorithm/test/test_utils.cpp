#include <gtest/gtest.h>
#include <vector>

#include <utils/general.h>


TEST(GENERAL, find_degee_of_graph) {

    // TODO(DEV): so many copy!
    std::vector<std::pair<matrix, size_t>> testset = {
        {matrix(1, {0}),                            0}, 
        {matrix(2, {0, 0, 0, 0}),                   0}, 
        {matrix(3, {0, 1, 1, 1, 0, 0, 1, 0, 0}),    2}, 
    };
  
    for (auto [adj, expected] : testset)
        EXPECT_EQ(find_degree_of_graph(adj), expected);
}

