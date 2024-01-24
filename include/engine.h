#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <fstream>
#include <iostream>

#include "csv.h"
#include "book.h"
#include "callbacks.h"

class Engine {
public:
    Engine(Callbacks* cb, uint64_t ltc) : callback(cb), latency(ltc) {}

    uint64_t mkt_buy(uint32_t size);
    uint64_t mkt_sell(uint32_t size);
    int run(const std::string& file, const std::string& instr_id);
private:
    Callbacks* callback;

    uint64_t latency;
    std::vector<Event> cbs;
    std::vector<Event> ops;

    uint64_t last_time;
    uint64_t next_order_id = 0;
};

#endif