#ifndef BULLET_H
#define BULLET_H

#include "Camera.h"
#include "Vector2.h"

#include <SDL2/SDL.h>

#include <SDL2/SDL_rect.h>
#include <cmath>

extern SDL_Texture* g_texBullet;

class Bullet {
    float m_angle = 0;
    Vector2 m_position;
    Vector2 m_velocity;
    bool m_isValid = true;
    float m_speed = 800.0f;

public:
    Bullet(double angle) {
        m_angle = angle;

        double radians = angle * M_PI / 180;   
        m_velocity.x = (float)std::cos(radians) * m_speed;
        m_velocity.y = (float)std::sin(radians) * m_speed;
    }

    ~Bullet();

    void setPosition(const Vector2& position) {
        m_position = position;
    }

    const Vector2& getPosition() const {
        return m_position;
    }

    void update(float deltaTime) {
        m_position += m_velocity * deltaTime;

        if (m_position.x <= 0 || m_position.x >= 1280 || m_position.y <= 0 || m_position.y >= 720) {
            m_isValid = false;
        }
    }

    void render(const Camera& camera) const {
        const SDL_FRect rectBullet = { m_position.x - 4, m_position.y - 2, 8, 4 };
        camera.renderTexture(g_texBullet, nullptr, &rectBullet, m_angle, nullptr);
    }

    void onHit() {
        m_isValid = false;
    }

    bool canRemove() const {
        return !m_isValid;
    }
};

#endif
