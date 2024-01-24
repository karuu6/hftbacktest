#include "engine.h"

uint64_t Engine::mkt_buy(uint32_t size) {
    Event op;
    op.action = 'M';
    op.size = size;
    op.side = 'B';
    op.id = next_order_id;
    op.time = last_time + latency;

    ops.emplace_back(op);
    return next_order_id++;
}

uint64_t Engine::mkt_sell(uint32_t size) {
    Event op;
    op.action = 'M';
    op.size = size;
    op.side = 'S';
    op.id = next_order_id;
    op.time = last_time + latency;

    ops.emplace_back(op);
    return next_order_id++;
}

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
        last_time = event.time;
        book.apply(event);

        for (const auto& cb : cbs) {
            if (event.time - cb.time > latency) {
                if (cb.action == 'T') {
                    Trade info;
                    info.price = event.price;
                    info.side = event.side;
                    info.time = event.time;
                    info.size = event.side;
                    callback->trade(info);
                }
            } else
                break;
        }

        if (event.action == 'T') {
            cbs.emplace_back(event);
        }

        uint32_t flags = std::stoul(row.at("flags"));
        if (!(flags & 128))
            continue;
        
        for (const auto& op : ops) {
            if (op.time <= event.time)
                break;
            
            if (op.action == 'M') {
                
            }
        }
    }

    return 0;
}