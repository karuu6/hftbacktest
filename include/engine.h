#ifndef ENGINE_H
#define ENGINE_H

#include "csv.h"
#include "book.h"
#include "callbacks.h"

#include <fstream>
#include <iostream>

class Engine {
public:
    Engine(Callbacks* cb) : callback(cb) {}
    int run(const std::string& file, const std::string& instr_id);
private:
    Callbacks* callback;
};

#endif