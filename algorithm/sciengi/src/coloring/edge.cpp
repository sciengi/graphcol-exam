#include <coloring/edge.h>



color_t get_available_color(const cmask_t& mask) {

    color_t color;
    for (color = 0; color < mask.size() and mask[color] == false; color++);
    return ++color;
}


int get_vertex_by_color(matrix& cmat, const cmask_t& mask, int base, color_t c) {

    int vertex = -1;
    if (mask[c - 1] == false) {
        for (vertex = 0; cmat[base][vertex] != c; vertex++);
    }

    return vertex;
}


// DEV: recoloring work for 'both directions' of edge
// TODO(DEV): sync for directed and not graphs
void color_edge(matrix& cmat, std::vector<cmask_t>& cmasks, int u, int v, color_t c, color_t CL) {

    color_t current = cmat[u][v];
    if (current != -1) {
        cmasks[u][current - 1] = true; 
        cmasks[v][current - 1] = true; 
    }

    cmat[u][v] = c;
    cmat[v][u] = c;

    if (c != CL) { // TODO(DEV): make variable CL from class `cmat`
        cmasks[u][c - 1] = false;
        cmasks[v][c - 1] = false;
    }
}


// DEV: assume that (base, next) = CL, add assert?
void build_fan(matrix& cmat, fan_t& fan, const std::vector<cmask_t>& cmasks, int base, int next) {

    fan.push_back(next); 
    cmask_t mask, base_mask = cmasks[base];
   
    std::unordered_set<color_t> viewed_colors;
    while (true) {
        mask = cmasks[next];
        color_t requested_color = get_available_color(mask);

        if (viewed_colors.count(requested_color) != 0) break;
        viewed_colors.insert(requested_color);

        next = get_vertex_by_color(cmat, base_mask, base, requested_color);
        if (next == -1) break;

        fan.push_back(next);
    }
}


void rotate_fan(matrix& cmat, std::vector<cmask_t>& cmasks, fan_t& fan, int base, int end, int CL) {

    color_t current_color = cmat[base][fan[end]];
    color_t new_color = CL;
    color_edge(cmat, cmasks, base, fan[end], new_color, CL);

    for (int i = end - 1; i >= 0; i--) {  // WARN(DEV): `i` can be negative if w = `end` = 0 (fan consist of 1 edge)
        new_color = current_color;
        current_color = cmat[base][fan[i]];
        color_edge(cmat, cmasks, base, fan[i], new_color, CL);
    }
}


void inverse_cd_path(matrix& cmat, std::vector<cmask_t>& cmasks, int base, color_t c, color_t d, color_t CL) {

    cmask_t mask = cmasks[base];

    int prev = base, next;
    base = get_vertex_by_color(cmat, mask, base, d);
    if (base == -1) return; // MATH: c and d not in base of fan

    int counter = 0;
    color_t path_colors[2] = {c, d};
    color_t current_color = path_colors[counter]; 

    while (1) {
        mask = cmasks[base];
        next = get_vertex_by_color(cmat, mask, base, current_color);

        if (next != -1)
            color_edge(cmat, cmasks, next, base, CL, CL);
        color_edge(cmat, cmasks, prev, base, current_color, CL);
        counter = ++counter % 2;
        current_color = path_colors[counter];

        if (next == -1) break;

        prev = base;
        base = next;
    }
}


matrix color_edges(matrix& adj) {
 
    auto report = start_report("report.json");

    size_t color_count = 1 + find_degree_of_graph(adj);
    cmask_t default_colors(color_count, true);

    /* TODO(MATH): Is it 100% that algoritm will use dG + 1 colors? *
     * (May it use dG colors for some graph? See C3 and P2)         */

    const auto vertex_count = adj.get_row_count();
    std::vector<cmask_t> cmasks(vertex_count, default_colors);


    const int NC = 0; 
    const int CL = -1;
    matrix cmat(vertex_count); 
    for (size_t i = 0; i < vertex_count; i++) {
        for (size_t j = 0; j < vertex_count; j++) {
            cmat[i][j] = adj[i][j] != NC ? CL : NC;
        }
    }
    
    // TODO(DEV): add copy constructors with fill/replace
    // TODO(DEV): generalize to better matrix class to solve NC/CL variable


    std::vector<int> fan;
    for (size_t i = 0; i < vertex_count; i++) {
        for (size_t j = 0; j < vertex_count; j++) {
            if (cmat[i][j] == NC or cmat[i][j] != CL) continue;

            add_record(report, cmat, "Step");

            build_fan(cmat, fan, cmasks, i, j);

            color_t c = get_available_color(cmasks[i]);
            color_t d = get_available_color(cmasks[fan.back()]);
            inverse_cd_path(cmat, cmasks, i, c, d, CL);

            size_t w = 0;
            while (get_available_color(cmasks[fan[w]]) != d) { w++; }

            rotate_fan(cmat, cmasks, fan, i, w, CL);
            color_edge(cmat, cmasks, i, fan[w], d, CL);

            fan.clear();
        }
    }


    add_record(report, cmat, "Finished");
    finish_report(report);

    return cmat; 
}


bool edge_coloring_is_correct(const matrix& cmat, int NC) {
    
    std::unordered_set<color_t> used_colors;
    for (size_t i = 0; i < cmat.get_row_count(); i++) {
        used_colors.clear();
        for (size_t j = 0; j < cmat.get_col_count(); j++) {
            if (cmat[i][j] == NC) continue;
            if (used_colors.count(cmat[i][j]) != 0) return false;
            used_colors.insert(cmat[i][j]);
        }
    }
    
    return true;
}


std::unordered_set<color_t> find_unique_colors(const matrix& cmat, int NC) {
    
    std::unordered_set<color_t> used_colors;
    for (size_t i = 0; i < cmat.get_row_count(); i++) {
        for (size_t j = 0; j < cmat.get_col_count(); j++) {
            if (cmat[i][j] == NC) continue;
            used_colors.insert(cmat[i][j]);
        }
    }

    return used_colors;
}
