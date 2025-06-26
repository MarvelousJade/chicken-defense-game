#ifndef CHICKEN_FAST_H
#define CHICKEN_FAST_H

#include "Chicken.h"

extern Atlas g_atlasChickenFast;

class ChickenFast : public Chicken {
public:
    ChickenFast() {
        p_animationRun.addFrame(&g_atlasChickenFast);

        p_speedrun = 80.0f;
    }

    ~ChickenFast() = default;
};

#endif
