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


// TODO(DEV): split to function with(for protocoling) and without(main version) `return`
path_t build_and_inverse_cd_path(matrix& cmat, std::vector<cmask_t>& cmasks, int base, color_t c, color_t d, color_t CL) {

    path_t path = { base };

    cmask_t mask = cmasks[base];

    int prev = base, next;
    base = get_vertex_by_color(cmat, mask, base, d);
    if (base == -1) return path; // MATH: c and d not in base of fan

    int counter = 0;
    color_t path_colors[2] = {c, d};
    color_t current_color = path_colors[counter]; 

    while (1) {
        path.push_back(base);

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

    return path;
}


matrix color_edges(matrix& adj) {
  
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

    using logger::opnames;
    using logger::targets;
    log_t lg = logger::start("report.json");
    
    logger::log(lg, opnames::UPDATE, targets::GRAPH, 
                "Дан граф", cmat);


    fan_t fan;
    path_t path;
    for (size_t i = 0; i < vertex_count; i++) {
        for (size_t j = 0; j < vertex_count; j++) {
            if (cmat[i][j] == NC or cmat[i][j] != CL) continue;

            logger::log(lg, opnames::SELECT, targets::EDGE, 
                    "Выбрано неокрашенное ребро", i, j);


            build_fan(cmat, fan, cmasks, i, j);

            logger::log(lg, opnames::SELECT, targets::FAN, 
                    "Построен максимальный веер", fan, i);


            color_t c = get_available_color(cmasks[i]);
            color_t d = get_available_color(cmasks[fan.back()]);

            path = build_and_inverse_cd_path(cmat, cmasks, i, c, d, CL);

            logger::log(lg, opnames::SELECT, targets::PATH, 
                    "Найден cd-путь", path);
            logger::log(lg, opnames::UPDATE, targets::GRAPH, 
                    "Путь инвертирован", cmat);


            size_t w = 0;
            while (get_available_color(cmasks[fan[w]]) != d) { w++; }
            
            logger::log(lg, opnames::SELECT, targets::VERTEX, 
                    "Освободилась вершина", fan[w]);

            fan_t smaller_fan(fan); // DEV: for logging purpose only
            smaller_fan.resize(w + 1);

            logger::log(lg, opnames::SELECT, targets::FAN, 
                    "Будет повернут веер", smaller_fan, i);

            rotate_fan(cmat, cmasks, fan, i, w, CL);

            logger::log(lg, opnames::UPDATE, targets::GRAPH, 
                    "Веер повернут", cmat);


            color_edge(cmat, cmasks, i, fan[w], d, CL);
            
            logger::log(lg, opnames::UPDATE, targets::GRAPH, 
                    "Покрашено новое ребро!", cmat);


            fan.clear();
        }
    }

    logger::log(lg, opnames::UPDATE, targets::GRAPH, 
            "Правильная раскраска ребер графа завершена!", cmat);

    logger::finish(lg);

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
