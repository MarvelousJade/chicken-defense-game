#ifndef CHICKEN_MEDIUM_H
#define CHICKEN_MEDIUM_H

#include "Chicken.h"

extern Atlas atlasChickenMedium;

class ChickenFast : public Chicken {
public:
    ChickenFast() {
        p_animationRun.addFrame(&atlasChickenMedium);

        p_speedrun = 50.0f;
    }

    ~ChickenFast() = default;
};

#endif
