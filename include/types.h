#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

struct Order;
struct Event;
struct Limit;

struct Event {
    uint64_t time;
    char action;
    char side;
    double price;
    uint32_t size;
    uint32_t id;
};

struct Order {      // resting order
    uint64_t time;
    double price;
    char side;
    uint32_t id;
    uint32_t size;
    Order* next = nullptr;
    Order* prev = nullptr;
    Limit* parent = nullptr;
};

struct Limit {     // price level
    double price;
    char side;
    uint32_t num;
    uint32_t size;
    Order* head = nullptr;
    Order* tail = nullptr;
};

#endif