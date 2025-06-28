#pragma once

#include <SFML/Graphics.hpp>

class EscapeUI {
private:
    bool escapeUiActive = false;

    float OPACITY_LEAP_SPEED = 5.0f;
    float currentOpacity = 0.0f;
    float targetOpacity  = 0.0f;
    sf::RectangleShape containBox;

    sf::RectangleShape footerBox;
    sf::Text footerText;

    int select = 0;
    float SELECT_LEAP_SPEED = 5.0f;
    float currentSaveBoxPosition   = 0.0f;
    float currentLoadBoxPosition   = 0.0f;
    float currentConfigBoxPosition = 0.0f;
    float currentQuitBoxPosition   = 0.0f;
    float targetSelectPosition     = 0.0f;
    float baseBoxPosition;
    sf::RectangleShape saveBox;
    sf::RectangleShape loadBox;
    sf::RectangleShape configBox;
    sf::RectangleShape quitBox;
    sf::Text saveText;
    sf::Text loadText;
    sf::Text configText;
    sf::Text quitText;

public:
    EscapeUI(const sf::Vector2f& size);

    bool isVisible() const;
    bool shouldDraw() const;
    
    void update(const float& dt, const sf::Vector2f& mousePosition);
    void updatePosition(const sf::Vector2f& size);
    int  handleClick(const sf::Vector2f& mousePosition);

    void toggle();
    void draw(sf::RenderTarget& target) const;

};