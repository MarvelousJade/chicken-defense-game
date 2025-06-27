#ifndef CHICKEN_MEDIUM_H
#define CHICKEN_MEDIUM_H

#include "Chicken.h"

extern Atlas g_atlasChickenMedium;

class ChickenMedium : public Chicken {
public:
    ChickenMedium() {
        p_animationRun.addFrame(&g_atlasChickenMedium);

        p_speedrun = 50.0f;
    }

    ~ChickenMedium() = default;
};

#endif
