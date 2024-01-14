#ifndef BOOK_H
#define BOOK_H

#include <map>
#include "types.h"

struct BidCmp {
    bool operator()(const double& lhs, const double& rhs) const {
        return lhs > rhs;
    }
};

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
    std::map<double, Limit*, BidCmp> bids;

    std::unordered_map<double, Limit*> limits;
    std::unordered_map<uint32_t, Order*> orders;

    void delete_order(uint32_t id);

};

#endif