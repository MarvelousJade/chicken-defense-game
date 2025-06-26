#ifndef CHICKEN_FAST_H
#define CHICKEN_FAST_H

#include "Chicken.h"

extern Atlas atlasChickenFast;

class ChickenFast : public Chicken {
public:
    ChickenFast() {
        p_animationRun.addFrame(&atlasChickenFast);

        p_speedrun = 80.0f;
    }

    ~ChickenFast() = default;
};

#endif
