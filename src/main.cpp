#include "engine.h"
#include <iostream>

class MyCB : public Callbacks {
    virtual void trade(Trade& info) {
        std::cout << info.price << std::endl;
    }
};

int main() {
    MyCB cb;
    Engine engine(&cb, 1000 * 1000);
    engine.run("glbx-mdp3-20240102.mbo.csv", "4120818");
    return 0;
}