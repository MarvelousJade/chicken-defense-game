#ifndef CHICKEN_SLOW_H
#define CHICKEN_SLOW_H

#include "Chicken.h"

extern Atlas g_atlasChickenSlow;

class ChickenSlow : public Chicken {
public:
    ChickenSlow() {
        p_animationRun.addFrame(&g_atlasChickenSlow);

        p_speedrun = 30.0f;
    }

    ~ChickenSlow() = default;
};

#endif
