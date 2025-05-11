#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>


class EdgeColoring {
private:
    int V;                               // Количество вершин
    std::vector<std::vector<int>> adj;   // Список смежности
    std::vector<std::vector<int>> color; // color[u][v] - цвет ребра u-v
    int max_color;                       // Максимальный используемый цвет (delta + 1)

public:
    EdgeColoring(int vertices) : V(vertices), adj(vertices), color(vertices, std::vector<int>(vertices, -1)) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int find_missing_color(int u) {
        std::unordered_set<int> used_colors;
        for (int v : adj[u]) {
            if (color[u][v] != -1) {
                used_colors.insert(color[u][v]);
            }
        }
        for (int c = 0; c <= max_color; ++c) {
            if (used_colors.find(c) == used_colors.end()) {
                return c;
            }
        }
        return max_color + 1;
    }

    std::vector<int> find_max_fan(int u, int x1) {
        std::vector<int> fan = {x1};
        std::unordered_set<int> in_fan = {x1};
        bool extended;

        int last = 0, c = 0;
        
        do {
            extended = false;
            last = fan.back();
            c = find_missing_color(last);
            
            for (int v : adj[u]) {
                if (in_fan.find(v) == in_fan.end() && color[u][v] == c) {
                    fan.push_back(v);
                    in_fan.insert(v);
                    extended = true;
                    break;
                }
            }
        } while (extended);
        
        return fan;
    }

    std::vector<int> find_cd_path(int u, int c, int d) {
        std::vector<int> path;
        int current = u;
        int prev = -1;
        int next_v = 0;
        
        bool is_c = true;
        
        while (true) {
            path.push_back(current);
            next_v = -1;
            
            for (int v : adj[current]) {
                if (v != prev && ((is_c && color[current][v] == c) || (!is_c && color[current][v] == d))) {
                    next_v = v;
                    break;
                }
            }
            
            if (next_v == -1) break;
            prev = current;
            current = next_v;
            is_c = !is_c;
        }
        
        return path;
    }

    void invert_cd_path(const std::vector<int>& path, int c, int d) {
        for (int i = 0; i < path.size() - 1; ++i) {
            int u = path[i], v = path[i+1];
            if (color[u][v] == c) {
                color[u][v] = d;
                color[v][u] = d;
            } else {
                color[u][v] = c;
                color[v][u] = c;
            }
        }
    }

    void rotate_fan(int u, std::vector<int>& fan, int d) {
        int w_pos = -1;
        int w = 0;

        bool d_missing = true;
        
        for (int i = 0; i < fan.size(); ++i) {
            w = fan[i];
            d_missing = true;
            for (int v : adj[w]) {
                if (color[w][v] == d) {
                    d_missing = false;
                    break;
                }
            }
            if (d_missing) {
                w_pos = i;
                break;
            }
        }
        
        if (w_pos == -1) return;
        
        for (int i = 0; i < w_pos; ++i) {
            color[u][fan[i]] = color[u][fan[i+1]];
            color[fan[i]][u] = color[u][fan[i+1]];
        }
        
        color[u][fan[w_pos]] = d;
        color[fan[w_pos]][u] = d;
    }

    void misra_gries_coloring() {
        int delta = 0;
        for (int u = 0; u < V; ++u) {
            delta = std::max(delta, (int)adj[u].size());
        }
        max_color = delta;
        
        std::vector<std::pair<int, int>> uncolored;
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                if (v > u && color[u][v] == -1) {
                    uncolored.emplace_back(u, v);
                }
            }
        }
        int u = 0, v = 0;
        std::pair<int, int> edge(0, 0);

        std::vector<int> fan;
        std::vector<int> cd_path;
        
        while (!uncolored.empty()) {
            edge = uncolored.back();
            uncolored.pop_back();
            
            u = edge.first;
            v = edge.second;
            
            if (color[u][v] != -1) continue;
            
            fan = find_max_fan(u, v);
            int c = find_missing_color(u);
            int d = find_missing_color(fan.back());
            
            cd_path = find_cd_path(u, c, d);
            invert_cd_path(cd_path, c, d);
            
            rotate_fan(u, fan, d);
        }
    }

    void print_colors() {
    std::cout << "    ";
    for (int i = 0; i < V; ++i) {
        std::cout << i << "  ";
    }
    std::cout << "\n";
    
    std::cout << "  +";
    for (int i = 0; i < V; ++i) {
        std::cout << "---";
    }
    std::cout << "\n";
    
    for (int u = 0; u < V; ++u) {
        std::cout << u << " |";
        for (int v = 0; v < V; ++v) {
            if (u == v) {
                std::cout << " - ";
            } else if (color[u][v] != -1) {
                if (color[u][v] < 10) 
                    std::cout << " ";
                std::cout << color[u][v] << " ";
            } else {
                std::cout << " - ";
            }
        }
        std::cout << "\n";
    }
}
};

int main() {
    EdgeColoring graph(5);
    
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    graph.add_edge(3, 4);
    
    graph.misra_gries_coloring();
    graph.print_colors();
    
    return 0;
}
