#ifndef CHICKEN_H
#define CHICKEN_H

#include "Camera.h"
#include "Vector2.h"
#include "Animation.h"

#include "SDL2/SDL_mixer.h"

extern Atlas atlasExplosion;

extern Mix_Chunk* soundExplosion;

class Chicken {
    Vector2 m_position;
    Animation m_animationExplosion;
    Animation* m_animationCurrent = nullptr;

    bool m_isAlive = true;
    bool m_isValid = true;

protected:
    float p_speedrun = 10.0f;
    Animation p_animationRun;

public:
    Chicken() {
        p_animationRun.setLoop(true);    
        p_animationRun.setInterval(0.1f);    

        m_animationExplosion.setLoop(false);
        m_animationExplosion.setInterval(0.08f);    
        m_animationExplosion.addFrame(&atlasExplosion);
        m_animationExplosion.setOnFinished(
            [&]() {
                m_isValid = false;
            }
        );
        m_position.x = 40.0f + (rand() % 1200);
        m_position.y = -50;
    }

    ~Chicken() = default;

    const Vector2& getPostion() const {
        return m_position;
    }

    void update(float deltaTime) {
        if (m_isAlive) m_position.y += p_speedrun * deltaTime;

        m_animationCurrent = (m_isAlive ? &p_animationRun : &m_animationExplosion);
        m_animationCurrent->setPosition(m_position);
        m_animationCurrent->update(deltaTime);
    }

    void render(const Camera& camera) const {
        m_animationCurrent->render(camera);
    }

    void onHurt() {
        m_isAlive = false;
        Mix_PlayChannel(-1, soundExplosion, 0);
    }

    void makeInvalid() {
        m_isValid = false;
    }

    bool checkAlive() {
        return m_isAlive;
    }

    bool canRemove() const {
        return !m_isAlive;
    }
};

#endif
