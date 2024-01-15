#include "engine.h"

int Engine::run(const std::string& file, const std::string& instr_id) {
    std::ifstream csv(file);

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
        if (!(row.at("instrument_id") == instr_id)) // "4120818"
            continue;
        Event event = encode_event(row);
        book.apply(event);

        if (event.action == 'T') {
            Trade info;
            info.price = event.price;
            info.side = event.side;
            info.size = event.size;
            info.time = event.time;

            callback->trade(info);
        }
    }

    return 0;
}