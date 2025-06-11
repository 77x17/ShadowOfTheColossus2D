#pragma once

#include <SFML/Graphics.hpp>

#include "Player.hpp"

class UI {
private:
    sf::Vector2f HEALTH_BAR_SIZE;
    sf::Vector2f HEALTH_BAR_POSITION;

    float TEXT_SIZE;

    sf::Text           healthPointsText;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;

public:
    UI();

    void update(const Player& player);

    void draw(sf::RenderWindow& window);

};