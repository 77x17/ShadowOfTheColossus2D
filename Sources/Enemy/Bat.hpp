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
private:
    float ATTACK_RANGE;
    
public:
    Bat(const float& x, const float& y);

    void followPlayer(const Player& player) override;

    void updateAnimation() override;

};