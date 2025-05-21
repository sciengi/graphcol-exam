#ifndef HPP_COLORING_TESTING
#define HPP_COLORING_TESTING

/* Header include `private` function to test them */

#include <coloring/edge.h>

bool edge_coloring_is_correct(const matrix& cmat, int NC);
std::unordered_set<color_t> find_unique_colors(const matrix& cmat, int NC); 


color_t get_available_color(const cmask_t& mask); 
int get_vertex_by_color(matrix& cmat, const cmask_t& mask, int base, color_t c);
void color_edge(matrix& cmat, std::vector<cmask_t>& cmasks, int u, int v, color_t c, color_t CL);


void build_fan(matrix& cmat, fan_t& fan, const std::vector<cmask_t>& cmasks, int base, int next);
void rotate_fan(matrix& cmat, std::vector<cmask_t>& cmasks, fan_t& fan, int base, int end, int CL);
void build_and_inverse_cd_path(matrix& cmat, std::vector<cmask_t>& cmasks, int base, color_t c, color_t d, color_t CL);


#endif
