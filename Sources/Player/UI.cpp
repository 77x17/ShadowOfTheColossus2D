#include "UI.hpp"

#include "Font.hpp"

UI::UI() {
    HEALTH_BAR_SIZE     = sf::Vector2f(200.0f, 20.0f);
    HEALTH_BAR_POSITION = sf::Vector2f(20.0f, 20.0f);
    
    TEXT_SIZE = 15.0f;

    healthPointsText.setFont(Font::font);
    healthPointsText.setCharacterSize(TEXT_SIZE);
    healthPointsText.setFillColor(sf::Color::White);
    healthPointsText.setPosition(HEALTH_BAR_POSITION + sf::Vector2f(0, -2.0f));

    healthBar.setSize(HEALTH_BAR_SIZE);
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(HEALTH_BAR_POSITION);

    healthBarBackground.setSize(HEALTH_BAR_SIZE + sf::Vector2f(4.0f, 4.0f));
    healthBarBackground.setFillColor(sf::Color(80, 80, 80, 100));
    healthBarBackground.setPosition(HEALTH_BAR_POSITION + sf::Vector2f(-2.0f, -2.0f));
}

void UI::update(const Player& player) {
    healthBar.setSize(sf::Vector2f(HEALTH_BAR_SIZE.x * player.getHealthStatus(), HEALTH_BAR_SIZE.y));
    player.getHealthPoints(healthPointsText);
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(healthPointsText);
}