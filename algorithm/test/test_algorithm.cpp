#include <gtest/gtest.h>
#include <vector>
#include <unordered_set>
#include <utils/general.h>

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


// TODO(DEV): move to utils testing
TEST(COLORING, edge_coloring_is_correct) {

    const int NC = 0;
    std::vector<std::pair<matrix, bool>> testset = {
        {matrix(1, {0}), true},
        {matrix(3, {0, 1, 2, 1, 0, 3, 2, 3, 0}), true},
        {matrix(3, {0, 1, 1, 1, 0, 0, 1, 0, 0}), false},
    };

    for (auto [cmat, expected] : testset)
        EXPECT_EQ(edge_coloring_is_correct(cmat, NC), expected);
}


TEST(COLORING, find_unique_colors) {
    const int NC = 0;
    std::vector<std::pair<matrix, std::unordered_set<int>>> testset = {
        {matrix(1, {0}), {}},
        {matrix(3, {0, 1, 2, 1, 0, 3, 2, 3, 0}), {1, 2, 3}},
        {matrix(3, {0, 1, 1, 1, 0, 0, 1, 0, 0}), {1}},
    };

    for (auto [cmat, expected] : testset)
        EXPECT_EQ(find_unique_colors(cmat, NC), expected);
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

    const int base = 0;
    const color_t CL = -1;
    std::vector<std::tuple<matrix, std::vector<cmask_t>, fan_t, int>> testset = {
        {
            matrix(2, {
                    0, CL,
                    CL, 0,
            }), 
            {
                {true, true}, 
                {true, true}, 
            }, 
            {1}, 0
        }, // Not rotated
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
            {1, 2, 3}, 2
        }, // Rotate full fan
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
            {1, 2, 3}, 1
        }, // Rotate part of fan
    };

    std::vector<color_t> fan_colors;
    for (auto& [cmat, cmasks, fan, end] : testset) {
        fan_colors.clear();
        for (size_t i = 1; i < fan.size(); i++) {
            fan_colors.push_back(cmat[base][fan[i]]);
        }

        rotate_fan(cmat, cmasks, fan, base, end, CL);
        for (size_t i = 0; i < end; i++) {
            ASSERT_EQ(cmat[base][fan[i]], fan_colors[i]);
        }

        ASSERT_EQ(cmat[base][fan[end]], CL);
    }
}


TEST(COLORING, build_and_inverse_cd_path) {

    const int base = 0;
    const color_t a = 1, d = 2, c = 3, b = 4, CL = -1; 

    matrix cmat(3, {
        0, d, a,
        d, 0, c,
        a, c, 0
    });

    std::vector<cmask_t> cmasks = {
        {false, false, true,  true},
        {true,  false, false, true},
        {false, true,  false, true},
    };

    matrix cd_cmat(3, {
        0, c, a,
        c, 0, d,
        a, d, 0
    });
    
    std::vector<cmask_t> cd_cmasks = {
        {false, true,  false, true},
        {true,  false, false, true},
        {false, false, true,  true},
    };

    matrix ba_cmat(3, {
        0, c, b,
        c, 0, d,
        b, d, 0
    });
    
    std::vector<cmask_t> ba_cmasks = {
        {true, true,  false, false},
        {true, false, false, true},
        {true, false, true,  false},
    };


    path_t path_cd = {0, 1, 2};
    path_t path_ba = {0, 2};
    path_t result;


    result = build_and_inverse_cd_path(cmat, cmasks, base, c, d, CL);
    EXPECT_EQ(path_cd, result);
    ASSERT_EQ(cmat, cd_cmat);
    ASSERT_EQ(cmasks, cd_cmasks);

    result = build_and_inverse_cd_path(cmat, cmasks, base, b, a, CL);
    EXPECT_EQ(path_ba, result);
    ASSERT_EQ(cmat, ba_cmat);
    ASSERT_EQ(cmasks, ba_cmasks);
}


TEST(COLORING, color_edges) {
   
    std::vector<matrix> testset = {
        matrix(1, {0}), // K_1
        matrix(3, {
                0, 1, 0,
                1, 0, 1,
                0, 1, 0,
        }), // P_3
        matrix(3, {
                0, 1, 1,
                1, 0, 1,
                1, 1, 0,
        }), // C_3
        matrix(4, {
                0, 1, 0, 0,
                1, 0, 1, 0,
                0, 1, 0, 1,
                0, 0, 1, 0,
        }), // P_4
        matrix(4, {
                0, 1, 0, 1,
                1, 0, 1, 0,
                0, 1, 0, 1,
                1, 0, 1, 0,
        }),// C_4
        matrix(4, {
                0, 1, 1, 1,
                1, 0, 0, 0,
                1, 0, 0, 0,
                1, 0, 0, 0,
        }), // T_4
        matrix(4, {
                0, 1, 1, 1,
                1, 0, 1, 1,
                1, 1, 0, 1,
                1, 1, 1, 0,
        }), // K_4
        matrix(5, {
                0, 0, 1, 1, 1,
                0, 0, 1, 1, 1,
                1, 1, 0, 0, 0,
                1, 1, 0, 0, 0,
                1, 1, 0, 0, 0,
        }), // K_2,3
        matrix(10, {
            //  0  1  2  3  4  5  6  7  8  9
                0, 1, 1, 0, 0, 1, 0, 0, 0, 0, // 0
                1, 0, 0, 0, 0, 0, 1, 1, 0, 0, // 1
                1, 0, 0, 1, 0, 0, 0, 0, 1, 0, // 2
                0, 0, 1, 0, 1, 0, 0, 1, 0, 0, // 3
                0, 0, 0, 1, 0, 1, 1, 0, 0, 0, // 4
                1, 0, 0, 0, 1, 0, 0, 0, 0, 1, // 5
                0, 1, 0, 0, 1, 0, 0, 0, 1, 0, // 6
                0, 1, 0, 1, 0, 0, 0, 0, 0, 1, // 7
                0, 0, 1, 0, 0, 0, 1, 0, 0, 1, // 8
                0, 0, 0, 0, 0, 1, 0, 1, 1, 0, // 9
        }), // Petersen graph
    };

    const int NC = 0;
    for (auto mat : testset) {
        matrix result = color_edges(mat);
        EXPECT_TRUE(edge_coloring_is_correct(result, NC));

        size_t deg = find_degree_of_graph(mat);
        auto colors = find_unique_colors(result, NC);
        EXPECT_TRUE(colors.size() <= deg + 1);
    }
}

