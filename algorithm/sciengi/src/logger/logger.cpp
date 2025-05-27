#include <logger/logger.h>

// DEV: HEADER ONLY LIBRARY!
#include <logger/dep/json.hpp>
using json = nlohmann::json;



void to_json(json& j, const matrix& mat) {
    
    std::vector<int> row(mat.get_col_count());
    for (size_t i = 0; i < mat.get_row_count(); i++) {
        row.assign(mat[i], mat[i + 1]);
        j.push_back(row);
    }
}



namespace logger {

log_t start(const char* filename) {
    std::fstream logfile(filename, std::ios::out);
    logfile << "[\n";

    return logfile; // DEV: will raise if log_t change
}


void finish(log_t& lg) {
    lg.seekp(lg.tellp() - static_cast<std::streamoff>(2L)); // DEV: replace comma, see log*
    lg << "\n]";
    lg.close();
}


void init_operation(json& op, opnames opname, targets target, const char* description) {
    op["opname"] = opname == UPDATE ? "update" : "select";
    op["description"] = description;
    switch(target) {
        case VERTEX:    op["target"] = "vertex";    break;
        case EDGE:      op["target"] = "edge";      break;
        case PATH:      op["target"] = "path";      break;
        case FAN:       op["target"] = "fan";       break;
        case GRAPH:     op["target"] = "graph";     break;
    };
}

// DEV: to change format of all log* functions
inline void log(log_t& lg, json& op) { lg << "    " << op << ",\n"; }


void log(log_t& lg, opnames opname, targets target, const char* description, const matrix& mat) {

    json op;
    init_operation(op, opname, target, description);

    op["data"] = mat;

    log(lg, op);
}


void log(log_t& lg, opnames opname, targets target, const char* description, int from, int to) {

    json op;
    init_operation(op, opname, target, description);

    op["data"] = {from , to};
    
    log(lg, op);
}


void log(log_t& lg, opnames opname, targets target, const char* description, fan_t fan, int base) {

    json op;
    init_operation(op, opname, target, description);

    std::vector<int> data = { base };
    data.insert(data.end(), fan.begin(), fan.end());
    
    op["data"] = data;
    
    log(lg, op);
}


void log(log_t& lg, opnames opname, targets target, const char* description, path_t path) {

    json op;
    init_operation(op, opname, target, description);

    op["data"] = path;
    
    log(lg, op);
}


void log(log_t& lg, opnames opname, targets target, const char* description, int vertex) {

    json op;
    init_operation(op, opname, target, description);

    op["data"] = vertex;
    
    log(lg, op);
}
}
