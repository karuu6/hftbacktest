#ifndef CSV_H
#define CSV_H

#include <map>
#include <vector>
#include <string>

#include "types.h"

const std::vector<std::string> parse_header(const std::string& line);
const std::map<std::string, std::string> parse_line(const std::string& line, const std::vector<std::string>& header);

Event encode_event(const std::map<std::string, std::string>& row);

#endif