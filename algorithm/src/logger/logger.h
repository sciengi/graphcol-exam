#ifndef HPP_LOGGER
#define HPP_LOGGER

#include <fstream>
#include <iomanip>
#include <vector>

typedef std::fstream log_t;

#include <coloring/edge.h>
#include <matrix/matrix.h>


namespace logger {

enum opnames {
    SELECT,
    UPDATE,
};

enum targets {
    VERTEX,
    EDGE,
    PATH,
    FAN,
    GRAPH
};

log_t start(const char* filename);
void finish(log_t& lg);

void log(log_t& lg, opnames opname, targets target, const char* description, const matrix& mat);
void log(log_t& lg, opnames opname, targets target, const char* description, int from, int to);
void log(log_t& lg, opnames opname, targets target, const char* description, fan_t fan, int base);
void log(log_t& lg, opnames opname, targets target, const char* description, path_t path);
void log(log_t& lg, opnames opname, targets target, const char* description, int vertex);

};


#endif
