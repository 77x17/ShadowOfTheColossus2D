#include "UI.hpp"

#include "Font.hpp"
#include "Quest.hpp"

UI::UI() {
    {
        HEALTH_BAR_SIZE         = sf::Vector2f(200.0f, 20.0f);
        HEALTH_BAR_POSITION     = sf::Vector2f(20.0f, 20.0f);
        HEALTH_POINTS_TEXT_SIZE = 16.0f;

        healthPointsText.setFont(Font::font);
        healthPointsText.setCharacterSize(HEALTH_POINTS_TEXT_SIZE);
        healthPointsText.setFillColor(sf::Color::White);
        healthPointsText.setOutlineColor(sf::Color::Black);
        healthPointsText.setOutlineThickness(1.0f);
        healthPointsText.setString("0.00/0.00");
        healthPointsText.setPosition(HEALTH_BAR_POSITION + sf::Vector2f(2.0f, HEALTH_BAR_SIZE.y / 2));
        healthPointsText.setOrigin(0, healthPointsText.getLocalBounds().top + healthPointsText.getLocalBounds().height / 2);

        healthBar.setSize(HEALTH_BAR_SIZE);
        healthBar.setFillColor(sf::Color::Red);
        healthBar.setPosition(HEALTH_BAR_POSITION);

        healthBarBackground.setSize(HEALTH_BAR_SIZE + sf::Vector2f(4.0f, 4.0f));
        healthBarBackground.setFillColor(sf::Color(80, 80, 80, 150));
        healthBarBackground.setPosition(HEALTH_BAR_POSITION + sf::Vector2f(-2.0f, -2.0f));
        healthBarBackground.setOutlineThickness(2.0f);
        healthBarBackground.setOutlineColor(sf::Color::Black);
    }

    {
        QUESTS_POSITION  = sf::Vector2f(20.f, 100.f);
        QUESTS_TEXT_SIZE = 12.5f;
        questsText.setFont(Font::font);
        questsText.setCharacterSize(QUESTS_TEXT_SIZE);
        questsText.setFillColor(sf::Color::White);
        questsText.setPosition(QUESTS_POSITION);

        questsBoxLabel.setFont(Font::font);
        questsBoxLabel.setCharacterSize(QUESTS_TEXT_SIZE);
        questsBoxLabel.setFillColor(sf::Color::Yellow);
        questsBoxLabel.setPosition(QUESTS_POSITION);
        questsBoxLabel.setString("--- [Quest] ---");
        questsBoxLabel.setOrigin(questsBoxLabel.getLocalBounds().left + questsBoxLabel.getLocalBounds().width / 2, 0);
        
        questsBox.setSize(questsText.getGlobalBounds().getSize() + sf::Vector2f(4.0f, 4.0f));
        questsBox.setFillColor(sf::Color(80, 80, 80, 150));
        questsBox.setPosition(QUESTS_POSITION + sf::Vector2f(-2.0f, -2.0f));
        questsBox.setOutlineThickness(2.0f);
        questsBox.setOutlineColor(sf::Color::Black);
    }
}

void UI::updateHealthBar(const Player& player) {
    healthBar.setSize(sf::Vector2f(HEALTH_BAR_SIZE.x * player.getHealthStatus(), HEALTH_BAR_SIZE.y));

    healthPointsText.setString(player.getHealthPoints());
}

void UI::updateQuests(const std::vector<Quest>& quests) {
    std::string display = "               \n";
    int idx = 0;
    for (const Quest& quest : quests) {
        display += quest.getObjectives(++idx);
    }

    questsText.setString(display);
    questsBox.setSize(questsText.getGlobalBounds().getSize() + sf::Vector2f(4.0f, 4.0f));
    questsBoxLabel.setPosition(QUESTS_POSITION + sf::Vector2f(questsText.getGlobalBounds().getSize().x / 2, 0));
}

void UI::update(const Player& player) {
    updateHealthBar(player);

    updateQuests(player.getQuests());
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(healthPointsText);

    window.draw(questsBox);
    window.draw(questsBoxLabel);
    window.draw(questsText);
}