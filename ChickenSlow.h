#ifndef CHICKEN_SLOW_H
#define CHICKEN_SLOW_H

#include "Chicken.h"

extern Atlas g_atlasChickenSlow;

class ChickenFast : public Chicken {
public:
    ChickenFast() {
        p_animationRun.addFrame(&g_atlasChickenSlow);

        p_speedrun = 30.0f;
    }

    ~ChickenFast() = default;
};

#endif
