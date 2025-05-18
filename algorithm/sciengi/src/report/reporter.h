#ifndef HPP_REPORTER
#define HPP_REPORTER


#include <fstream>
#include <string_view>
#include <matrix/matrix.h>


std::fstream start_report(std::string_view name);

void add_record(std::fstream& r, const matrix& mat, std::string_view description);

void finish_report(std::fstream& r);

#endif
