#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.hpp"

enum class BatState {
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT
};

class Bat : public Enemy {
public:
    Bat(float x, float y);

    void updateAnimation() override;

};