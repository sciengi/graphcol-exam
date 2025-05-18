#include "reporter.h"

// DEV: HEADER ONLY LIBRARY
#include <boost/json/src.hpp>
using namespace boost;



std::fstream start_report(std::string_view name) {
    std::fstream report(name.data(), std::fstream::out);
    report << '[';
    return report;
}


void add_record(std::fstream& report, const matrix& mat, std::string_view description) {

    json::object frame;
    frame["matrix"] = json::value_from(mat);
    frame["description"] = description; 

    report << json::serialize(frame) << ',';
}


void finish_report(std::fstream& report) {
    report.seekp(report.tellp() - 1L);
    report << ']';
    report.close();
}
