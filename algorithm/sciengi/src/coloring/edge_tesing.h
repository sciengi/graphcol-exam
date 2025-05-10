#ifndef HPP_COLORING_TESTING
#define HPP_COLORING_TESTING

/* Header include `private` function to test them */

#include <coloring/edge.h>

color_t get_available_color(const cmask_t& mask); 
int get_vertex_by_color(matrix& cmat, const cmask_t& mask, int base, color_t c);

void color_edge(matrix& cmat, std::vector<cmask_t>& cmasks, int u, int v, color_t c, color_t CL);

void build_fan(matrix& cmat, fan_t& fan, const std::vector<cmask_t>& cmasks, int base, int next);
void rotate_fan(matrix& cmat, std::vector<cmask_t>& cmasks, fan_t& fan, int base, int end, int CL);

void inverse_cd_path(matrix& cmat, std::vector<cmask_t> cmasks, int base, color_t c, color_t d, color_t CL);


#endif
