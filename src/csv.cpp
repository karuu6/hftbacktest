#include "csv.h"

const std::vector<std::string> parse_header(const std::string& line) {
    char sep = ',';
    size_t strpos = 0;
    size_t endpos = line.find(sep);

    std::vector<std::string> header;

    while (endpos < line.length()) {
        header.emplace_back(line.substr(strpos, endpos - strpos));
        strpos = endpos + 1;
        endpos = line.find(sep, strpos);
    }
    header.emplace_back(line.substr(strpos));

    return header;
}

const std::map<std::string, std::string> parse_line(const std::string& line, const std::vector<std::string>& header) {
    char sep = ',';
    size_t strpos = 0;
    size_t endpos = line.find(sep);

    std::map<std::string, std::string> row;

    for (size_t i = 0; i < header.size(); ++i) {
        row.emplace(header.at(i), line.substr(strpos, endpos - strpos));
        strpos = endpos + 1;
        endpos = line.find(sep, strpos);
    }

    return row;
}

Event encode_event(const std::map<std::string, std::string>& row) {
    Event event;
    event.time = std::stol(row.at("ts_event"));
    event.action = row.at("action")[0];
    event.side = row.at("side")[0];
    event.price = std::stod(row.at("price"));
    event.size = std::stol(row.at("size"));
    event.id = std::stol(row.at("order_id"));

    return event;
}