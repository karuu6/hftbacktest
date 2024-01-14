#include <fstream>
#include <iostream>
#include "csv.h"
#include "book.h"

int main() {
    std::ifstream csv("glbx-mdp3-20240102.mbo.csv");

    if (!csv) {
        std::cerr << "error opening csv file" << std::endl;
        return 1;
    }

    std::string header_line;
    getline(csv, header_line);
    auto header = parse_header(header_line);

    Book book;
    std::string line;

    while(std::getline(csv, line)) {
        std::map<std::string, std::string> row = parse_line(line, header);
        if (!(row.at("instrument_id") == "4120818"))
            continue;
        Event event = encode_event(row);
        book.apply(event);
    }

    return 0;
}