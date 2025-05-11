#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>

#include <coloring/edge_tesing.h>



TEST(COLORING, get_available_color) {

    std::vector<std::pair<cmask_t, color_t>> testset = {
        {{true, true, true},    1},
        {{false, true, false},  2},
        {{false, true, true},   2},
        {{false, false, false}, 4}, // MATH: unreachable in algorithm, 
                                    // can be used to check errors
    };

    for (auto& [mask, expected] : testset)
        EXPECT_EQ(get_available_color(mask), expected);
}


TEST(COLORING, get_vertex_by_color) {

    std::vector<std::tuple<matrix, cmask_t, int, color_t, int>> testset = {
        {
            matrix(1, {0}), 
            {true},        // Must be not empty
            0, 1, -1
        },
        {
            matrix(2, {0, 1, 1, 0}), 
            {false}, 
            0, 1, 1
        },
        {
            matrix(3, {0, 1, 2, 1, 0, 3, 2, 3, 0}), 
            {false, false}, 
            0, 1, 1
        },
        {
            matrix(3, {0, 1, 1, 1, 0, 2, 1, 2, 0}), 
            {false, true},  
            0, 1, 1
        }, 
        // MATH: last unreachable in algorithm,
        // every step builds correct edge coloring
        // Not managed uncorrect color input
    };

    for (auto& [cmat, mask, base, color, expected] : testset)
        EXPECT_EQ(get_vertex_by_color(cmat, mask, base, color), expected);
}


TEST(COLORING, color_edge) {

    const color_t CL = -1;
    std::vector<std::tuple<matrix, std::vector<cmask_t>, int, int, color_t>> testset = {
        {
            matrix(2, {0, CL, CL, 0}),
            {
                {true, true},
                {true, true},
            },
            0, 1,
            1
        }, // CL -> 1
        {
            matrix(2, {0, 1, 1, 0}),
            {
                {false, true},
                {false, true},
            },
            0, 1,
            2
        }, // 1 -> 2
        {
            matrix(2, {0, 1, 1, 0}),
            {
                {false, true},
                {false, true},
            },
            0, 1,
            CL
        }, // 1 -> CL
    };

    for (auto& [cmat, cmasks, u, v, color] : testset) {
        color_t prev = cmat[u][v];
        color_edge(cmat, cmasks, u, v, color, CL);

        EXPECT_EQ(cmat[u][v], color);
        EXPECT_EQ(cmat[v][u], color);

        if (color != CL) {
            EXPECT_FALSE(cmasks[u][color - 1]);
            EXPECT_FALSE(cmasks[v][color - 1]);
        }
        
        if (prev != CL) {
            EXPECT_TRUE(cmasks[u][prev - 1]);
            EXPECT_TRUE(cmasks[v][prev - 1]);
        }

        // TODO(DEV): 
        // - make easer function or split
        // - add CL to cmask_t as zero?
    }
}


TEST(COLORING, build_fan) {
   
    const color_t CL = -1;
    std::vector<std::tuple<matrix, std::vector<cmask_t>, int, int>> testset = {
        {
            matrix(3, {0, CL, 1, CL, 0, 0, 1, 0, 0}), 
            {
                {false, true, true}, 
                {true, true, true},
                {false, true, true}
            },
            0, 1
        }, // No color conflict
        {
            matrix(4, {
                    0, CL, 1, 2, 
                    CL, 0, 0, 0, 
                    1,  0, 0, 0, 
                    2,  0, 0, 0
            }), 
            {
                {false, false, true, true}, 
                {true,  true,  true, true}, 
                {false, true,  true, true}, 
                {true,  false, true, true}, 
            },
            0, 1
        }, // Color conflict
    };

    for (auto& [cmat, cmasks, u, v] : testset) {
        fan_t fan;
        build_fan(cmat, fan, cmasks, u, v);

        ASSERT_TRUE(not fan.empty());
        ASSERT_TRUE(fan[0] == v);
        ASSERT_TRUE(cmat[u][fan[0]] == CL); // Top level problem

        std::unordered_set<color_t> viewed_colors;
        for (size_t i = 1; i < fan.size(); i++) {
            color_t fan_edge_color = cmat[u][fan[i]];
            ASSERT_TRUE(fan_edge_color == get_available_color(cmasks[fan[i - 1]]));  
            ASSERT_TRUE(viewed_colors.count(fan_edge_color) == 0);
            viewed_colors.insert(fan_edge_color);
        }
        
        color_t last_fan_edge_color = get_available_color(cmasks[fan.back()]);
        ASSERT_TRUE(
                cmasks[u][last_fan_edge_color - 1] == true 
                or viewed_colors.count(last_fan_edge_color) != 0
        ); 
    }
}


TEST(COLORING, rotate_fun) {

}


TEST(COLORING, inverse_cd_path) {

}


TEST(COLORING, color_edges) {
    
}

