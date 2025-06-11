#pragma once

#include <SFML/Graphics.hpp>

#include "Player.hpp"

class UI {
private:
    sf::Vector2f       HEALTH_BAR_SIZE;
    sf::Vector2f       HEALTH_BAR_POSITION;
    float              HEALTH_POINTS_TEXT_SIZE;
    sf::Text           healthPointsText;
    sf::RectangleShape healthBar;
    sf::RectangleShape healthBarBackground;
    
    float              QUESTS_TEXT_SIZE;
    sf::Vector2f       QUESTS_POSITION;
    sf::Text           questsBoxLabel;
    sf::Text           questsText;
    sf::RectangleShape questsBox;

public:
    UI();

    void updateHealthBar(const Player& player);
    void updateQuests(const std::vector<Quest>& quests);
    void update(const Player& player);

    void draw(sf::RenderWindow& window);

};