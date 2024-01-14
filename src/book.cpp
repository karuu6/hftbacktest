#include "book.h"
#include <iostream>

bool Book::apply(const Event& event) {
    switch (event.action) {
    case 'A':
        add(event);
        break;
    
    case 'R':
        clear();
        break;
    
    case 'M':
        modify(event);
        break;
    
    case 'C':
        cancel(event);
        break;

    default:
        return false;
    };

    return true;
}

void Book::add(const Event& event) {
    Order* order = new Order();
    order->time = event.time;
    order->price = event.price;
    order->side = event.side;
    order->id = event.id;
    order->size = event.size;

    orders.emplace(order->id, order);

    Limit* limit = nullptr;
    auto range = limits.equal_range(order->price);
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second->side == order->side) {
            limit = it->second;
            break;
        }
    }

    if (limit) {
        order->parent = limit;
        order->prev = limit->tail;
        limit->tail->next = order;
        limit->tail = order;

        limit->num += 1;
        limit->size += order->size;
    } else {
        limit = new Limit();
        limit->price = order->price;
        limit->side = order->side;
        limit->num = 1;
        limit->size = order->size;
        limit->head = order;
        limit->tail = order;

        order->parent = limit;
        limits.emplace(order->price, limit);

        if (order->side == 'B')
            bids.emplace(order->price, limit);
        else
            asks.emplace(order->price, limit);
    }
}

void Book::modify(const Event& event) {
    assert(orders.find(event.id) != orders.end() && "order exists");
    Order* order = orders.at(event.id);
    if (event.price != order->price || event.size >= order->size) { // order loses priority if price changes or size increases
        delete_order(order->id);
        add(event);
    } else {
        order->parent->size -= order->size - event.size;
        order->size = event.size;
        order->time = event.time;
    }
}

void Book::cancel(const Event& event) {
    assert(orders.find(event.id) != orders.end() && "order exists");
    Order* order = orders.at(event.id);
    assert(event.size <= order->size && "cancel size <= order size");

    if (order->size == event.size)
        delete_order(order->id);
    else {
        order->size -= event.size;
        order->parent->size -= event.size;
    }
}

void Book::clear() {
    for (const auto& kv : limits)
        delete kv.second;
    for (const auto& kv : orders)
        delete kv.second;
    
    asks.clear();
    bids.clear();
    orders.clear();
    limits.clear();
}

void Book::delete_order(uint64_t id) {
    assert(orders.find(id) != orders.end() && "order exists");
    Order* order = orders.at(id);
    if (order->next)
        order->next->prev = order->prev;
    if (order->prev)
        order->prev->next = order->next;
    if (order->parent->head == order)
        order->parent->head = order->next;
    if (order->parent->tail == order)
        order->parent->tail = order->prev;
    
    order->parent->num -= 1;
    order->parent->size -= order->size;

    if (order->parent->num == 0) {
        auto range = limits.equal_range(order->price);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == order->parent) {
                limits.erase(it);
                break;
            }
        }
        if (order->side == 'B')
            bids.erase(order->price);
        else
            asks.erase(order->price);
        delete order->parent;
    }

    orders.erase(id);
    delete order;
}

double Book::cost_buy(uint32_t size) const {
    double cost = 0;
    const auto it = asks.begin();
    while (size > 0 && it != asks.end()) {
        uint32_t qty = std::min(size, it->second->size);
        cost += qty * it->first;
        size -= qty;
    }
    return cost;
}

double Book::cost_sell(uint32_t size) const {
    double cost = 0;
    const auto it = bids.begin();
    while (size > 0 && it != asks.end()) {
        uint32_t qty = std::min(size, it->second->size);
        cost += qty * it->first;
        size -= qty;
    }
    return cost;
}

Book::~Book() {
    clear();
}