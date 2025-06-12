#include "UI.hpp"

#include "Font.hpp"
#include "Quest.hpp"

UI::UI() {
    PADDING               = sf::Vector2f(2.0f, 2.0f);
    BOX_OUTLINE_THICKNESS = 2.0f;

    {
        LEVEL_TEXT_SIZE       = 20.0f;
        LEVEL_BACKGROUND_SIZE = sf::Vector2f(70.0f, 35.0f);
        LEVEL_POSISION        = sf::Vector2f(20.0f, 20.0f);
        
        levelText.setFont(Font::font);
        levelText.setCharacterSize(LEVEL_TEXT_SIZE);
        levelText.setFillColor(sf::Color::White);
        levelText.setOutlineColor(sf::Color::Black);
        levelText.setOutlineThickness(0.5f);
        levelText.setString("Lv.01");
        levelText.setPosition(LEVEL_POSISION + LEVEL_BACKGROUND_SIZE / 2.0f);
        levelText.setOrigin(levelText.getLocalBounds().left + levelText.getLocalBounds().width / 2, levelText.getLocalBounds().top + levelText.getLocalBounds().height / 2);

        levelBackground.setSize(LEVEL_BACKGROUND_SIZE + PADDING * 2.0f);
        levelBackground.setPosition(LEVEL_POSISION - PADDING);
        levelBackground.setFillColor(sf::Color(80, 80, 80, 150));
        levelBackground.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        levelBackground.setOutlineColor(sf::Color::Black);
    }
    {
        HEALTH_POINTS_TEXT_SIZE = 16.0f;
        HEALTH_BAR_SIZE         = sf::Vector2f(200.0f, 20.0f);
        HEALTH_BAR_POSITION     = sf::Vector2f(100.0f, 20.0f);
        HEALTH_BAR_LEAP_SPEED   = 2.0f;
        healthBarRatio          = 1.0f;

        healthPointsText.setFont(Font::font);
        healthPointsText.setCharacterSize(HEALTH_POINTS_TEXT_SIZE);
        healthPointsText.setFillColor(sf::Color::White);
        healthPointsText.setOutlineColor(sf::Color::Black);
        healthPointsText.setOutlineThickness(0.5f);
        healthPointsText.setString("0.00/0.00");
        healthPointsText.setPosition(HEALTH_BAR_POSITION + sf::Vector2f(PADDING.x, HEALTH_BAR_SIZE.y / 2));
        healthPointsText.setOrigin(0, healthPointsText.getLocalBounds().top + healthPointsText.getLocalBounds().height / 2);
        
        healthBar.setSize(HEALTH_BAR_SIZE);
        healthBar.setPosition(HEALTH_BAR_POSITION);
        healthBar.setFillColor(sf::Color::Red);

        healthBarBackground.setSize(HEALTH_BAR_SIZE + PADDING * 2.0f);
        healthBarBackground.setPosition(HEALTH_BAR_POSITION - PADDING);
        healthBarBackground.setFillColor(sf::Color(80, 80, 80, 150));
        healthBarBackground.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        healthBarBackground.setOutlineColor(sf::Color::Black);
    }
    {
        XP_BAR_SIZE       = sf::Vector2f(200.0f, 5.0f);
        XP_BAR_POSISION   = HEALTH_BAR_POSITION + sf::Vector2f(0.0f, HEALTH_BAR_SIZE.y + 10.0f);
        XP_BAR_LEAP_SPEED = 2.0f;
        XPProgress        = 0.0f;
        
        XPBar.setSize(XP_BAR_SIZE);
        XPBar.setPosition(XP_BAR_POSISION);
        XPBar.setFillColor(sf::Color::Green);

        XPBarBackground.setSize(XP_BAR_SIZE + PADDING * 2.0f);
        XPBarBackground.setPosition(XP_BAR_POSISION - PADDING);
        XPBarBackground.setFillColor(sf::Color(80, 80, 80, 150));
        XPBarBackground.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        XPBarBackground.setOutlineColor(sf::Color::Black);
    }
    {
        QUESTS_TEXT_SIZE      = 12.5f;
        QUESTS_PADDING        = sf::Vector2f(20.f, 0.0f);
        QUESTS_BOX_LEAP_SPEED = 2.0f;
        questsPosition        = sf::Vector2f(0.0f, 0.0f);

        questsText.setFont(Font::font);
        questsText.setCharacterSize(QUESTS_TEXT_SIZE);
        questsText.setFillColor(sf::Color::White);
        questsText.setString("               \n");

        questsBoxLabel.setFont(Font::font);
        questsBoxLabel.setCharacterSize(QUESTS_TEXT_SIZE);
        questsBoxLabel.setFillColor(sf::Color::Yellow);
        questsBoxLabel.setString("--- [Quest] ---\n");
        questsBoxLabel.setOrigin(questsBoxLabel.getLocalBounds().left + questsBoxLabel.getLocalBounds().width / 2, 0);
        
        questsBoxSize = questsText.getGlobalBounds().getSize();
        questsBox.setSize(questsBoxSize + PADDING * 2.0f);
        questsBox.setFillColor(sf::Color(80, 80, 80, 150));
        questsBox.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        questsBox.setOutlineColor(sf::Color::Black);
    }
}

void UI::updateHealthBar(const float& dt, const Player& player) {
    healthPointsText.setString(player.getHealthPoints());
    
    healthBarRatio += (player.getHealthRatio() - healthBarRatio) * HEALTH_BAR_LEAP_SPEED * dt;

    healthBar.setSize(sf::Vector2f(HEALTH_BAR_SIZE.x * healthBarRatio, HEALTH_BAR_SIZE.y));
}

void UI::updateLevelAndXP(const float& dt, const Player& player) {
    levelText.setString("Lv." + std::to_string(player.getLevel()));

    XPProgress += (player.getXPRatio() - XPProgress) * XP_BAR_LEAP_SPEED * dt;

    XPBar.setSize(sf::Vector2f(XP_BAR_SIZE.x * XPProgress, XP_BAR_SIZE.y));
}

void UI::updateQuests(const float& dt, const std::vector<Quest>& quests, const sf::Vector2f& uiSize) {
    std::string display = "               \n\n";
    int idx = 0;
    for (const Quest& quest : quests) {
        display += quest.getObjectives(++idx) + '\n';
    }
    display.pop_back();
    questsText.setString(display);
    
    sf::FloatRect textBounds = questsText.getGlobalBounds();
    sf::Vector2f targetQuestsPosition = sf::Vector2f(uiSize.x - textBounds.getSize().x, uiSize.y / 2 - textBounds.getSize().y / 2) - QUESTS_PADDING;
    questsPosition += (targetQuestsPosition - questsPosition) * QUESTS_BOX_LEAP_SPEED * dt;
    questsBoxSize  += (textBounds.getSize() - questsBoxSize) * QUESTS_BOX_LEAP_SPEED * dt;
 
    questsBox.setSize(questsBoxSize + PADDING * 2.0f);
    
    questsText.setPosition(questsPosition);
    questsBox.setPosition(questsPosition - PADDING);
    questsBoxLabel.setPosition(questsPosition + sf::Vector2f(questsBoxSize.x / 2, 0));
}

void UI::update(const float& dt, const Player& player, const sf::Vector2f& uiSize) {
    updateHealthBar(dt, player);

    updateLevelAndXP(dt, player);

    updateQuests(dt, player.getQuests(), uiSize);
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(levelBackground);
    window.draw(levelText);

    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(healthPointsText);

    window.draw(XPBarBackground);
    window.draw(XPBar);

    window.draw(questsBox);
    window.draw(questsBoxLabel);
    
    sf::Vector2f targetQuestsBoxSize = questsText.getLocalBounds().getSize();
    if (questsBoxSize.x >= targetQuestsBoxSize.y - PADDING.x * 2 && questsBoxSize.y >= targetQuestsBoxSize.y - PADDING.y * 2) {
        window.draw(questsText);
    }
}