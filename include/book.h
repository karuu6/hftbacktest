#ifndef BOOK_H
#define BOOK_H

#include <map>
#include "types.h"

class Book {
public:
    bool apply(const Event& event);

    void add(const Event& event);
    void modify(const Event& event);
    void cancel(const Event& event);
    void clear();

    void print_book() const;

    ~Book();

private:
    std::map<double, Limit*> asks;
    std::map<double, Limit*, std::greater<double>> bids;

    std::unordered_map<uint64_t, Order*> orders;
    std::unordered_multimap<double, Limit*> limits;

    void delete_order(uint64_t id);
};

#endif