#include "book.h"

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

    if (limits.find(order->price) == limits.end()) {
        Limit* limit = new Limit();
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
    } else {
        Limit* limit = limits.at(order->price);
        assert(limit->side == order->side && "order side matches existing limit side");

        order->parent = limit;
        order->prev = limit->tail;
        limit->tail->next = order;
        limit->tail = order;

        limit->num += 1;
        limit->size += order->size;
    }

    orders.emplace(order->id, order);
}

void Book::modify(const Event& event) {
    return;
}

void Book::cancel(const Event& event) {
    return;
}

void Book::clear() {
    return;
}

Book::~Book() {
    for (const auto& kv : limits)
        delete kv.second;
    for (const auto& kv : orders)
        delete kv.second;
}