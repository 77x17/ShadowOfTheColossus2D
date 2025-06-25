#pragma once

#include <SFML/Graphics.hpp>

#include "Enemy.hpp"

enum class SlimeState {
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT
};

class Slime : public Enemy {
private:
    float ATTACK_RANGE;
    
public:
    Slime(const sf::Vector2f& position, 
        const std::string&    nameAndLevel, 
        const float&          hp, 
        const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory);

    void followPlayer(const Player& player) override;

    void updateTimer(const float& dt) override;
    void updateAnimation() override;

};