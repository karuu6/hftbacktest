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

void Book::delete_order(uint32_t id) {
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
        limits.erase(order->price);
        if (order->side == 'B')
            bids.erase(order->price);
        else
            asks.erase(order->price);
        delete order->parent;
    }

    orders.erase(id);
    delete order;
}

Book::~Book() {
    clear();
}