#include "UI.hpp"

#include "Font.hpp"
#include "Quest.hpp"
#include "Constants.hpp"

#include <cmath>

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
        XP_TEXT_SIZE      = 10.0f;
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

        XPText.setFont(Font::font);
        XPText.setCharacterSize(XP_TEXT_SIZE);
        XPText.setFillColor(sf::Color::Black);
        XPText.setOutlineColor(sf::Color::White);
        XPText.setOutlineThickness(0.5f);
        XPText.setString("0.00/0.00");
        XPText.setPosition(XP_BAR_POSISION + sf::Vector2f(XP_BAR_SIZE.x / 2, XP_BAR_SIZE.y + PADDING.y * 2));
        XPText.setOrigin(XPText.getLocalBounds().left + XPText.getLocalBounds().width / 2, 0.0f);
    }
    {
        QUESTS_TEXT_SIZE      = 12.5f;
        QUESTS_PADDING        = sf::Vector2f(20.f, 0.0f);
        QUESTS_BOX_LEAP_SPEED = 2.0f;
        questsPosition        = sf::Vector2f(WINDOW_WIDTH, 0);

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

        questsBoxVisible = false;
    }
    {
        MINIMAP_PADDING        = sf::Vector2f(20.0f, 20.0f);
        MINIMAP_LEAP_SPEED     = 2.0f;
        MINIMAP_SIZE           = sf::Vector2f(200, 200);
        minimapSize            = sf::Vector2f(200, 200);
        lastMinimapTextureSize = sf::Vector2u(200, 200);
        minimapViewRatio       = 3.0f;
        minimapPosition        = sf::Vector2f(WINDOW_WIDTH, 0);
        lastMinimapPosition    = sf::Vector2f(WINDOW_WIDTH, 0);

        minimapTexture.create(static_cast<unsigned int>(minimapSize.x), static_cast<unsigned int>(minimapSize.y));
        minimapSprite.setTexture(minimapTexture.getTexture());
        minimapView.setSize(minimapSize * minimapViewRatio);
        minimapView.setCenter(0.0f, 0.0f);

        minimapBox.setSize(sf::Vector2f(minimapSize) + PADDING * 2.0f);
        minimapBox.setFillColor(sf::Color(80, 80, 80, 150));
        minimapBox.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        minimapBox.setOutlineColor(sf::Color::Black);

        fullMinimapActive = false;
    }
}

void UI::updateHealthBar(const float& dt, const Player& player) {
    healthPointsText.setString(player.getHealthPointsString());
    
    healthBarRatio += (player.getHealthRatio() - healthBarRatio) * HEALTH_BAR_LEAP_SPEED * dt;

    healthBar.setSize(sf::Vector2f(HEALTH_BAR_SIZE.x * healthBarRatio, HEALTH_BAR_SIZE.y));
}

void UI::updateLevelAndXP(const float& dt, const Player& player) {
    levelText.setString("Lv." + std::to_string(player.getLevel()));

    XPText.setString(player.getXPString());
    XPText.setOrigin(XPText.getLocalBounds().left + XPText.getLocalBounds().width / 2, 0.0f);

    XPProgress += (player.getXPRatio() - XPProgress) * XP_BAR_LEAP_SPEED * dt;

    XPBar.setSize(sf::Vector2f(XP_BAR_SIZE.x * XPProgress, XP_BAR_SIZE.y));
}

void UI::updateQuests(const float& dt, const std::vector<Quest>& quests, const sf::Vector2f& uiSize) {
    std::string display = "               \n";

    if (questsBoxVisible) {
        int idx = 0;
        for (const Quest& quest : quests) {
            display += quest.getQuestInformation(++idx) + '\n';
        }
        display.pop_back();

        questsBoxLabel.setFillColor(sf::Color::Yellow);
    }
    else {
        questsBoxLabel.setFillColor(sf::Color::White);
    }

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

void UI::updateMinimap(const float& dt, const Player& player, const sf::Vector2f& uiSize, const TileMap& map) {
    sf::Vector2f targetSize, targetPosition;
    if (!fullMinimapActive) {
        targetSize     = sf::Vector2f(MINIMAP_SIZE);
        targetPosition = sf::Vector2f(uiSize.x - MINIMAP_SIZE.x - MINIMAP_PADDING.x, MINIMAP_PADDING.y);
    }
    else {
        targetSize     = sf::Vector2f(uiSize - MINIMAP_PADDING * 2.0f);
        targetPosition = sf::Vector2f(MINIMAP_PADDING.x, MINIMAP_PADDING.y);
    }

    float SNAP_THRESHOLD = 5.0f;
    sf::Vector2f distToTarget = targetPosition - minimapPosition;
    if (distToTarget != sf::Vector2f(0, 0) && std::abs(distToTarget.x) < SNAP_THRESHOLD && std::abs(distToTarget.y) < SNAP_THRESHOLD) {
        minimapSize     = targetSize;
        minimapPosition = targetPosition;
    }
    else {
        minimapSize     += (targetSize     - minimapSize)     * MINIMAP_LEAP_SPEED * dt;
        minimapPosition += (targetPosition - minimapPosition) * MINIMAP_LEAP_SPEED * dt;
    }

    sf::Vector2u newTextureSize = {
        static_cast<unsigned int>(std::round(minimapSize.x)),
        static_cast<unsigned int>(std::round(minimapSize.y))
    };

    if (newTextureSize != lastMinimapTextureSize && newTextureSize.x > 0 && newTextureSize.y > 0) {
        minimapTexture.create(newTextureSize.x, newTextureSize.y);
        minimapTexture.setSmooth(false); 
        lastMinimapTextureSize = newTextureSize;
    }

    if (lastMinimapTextureSize.x == 0 || lastMinimapTextureSize.y == 0) {
        return;
    }

    minimapSprite.setTexture(minimapTexture.getTexture(), true);

    minimapView.setSize(sf::Vector2f(lastMinimapTextureSize) * minimapViewRatio);
    sf::Vector2f center = player.getCenterPosition();
    center.x = std::floor(center.x);
    center.y = std::floor(center.y);
    minimapView.setCenter(center);

    minimapTexture.clear(sf::Color(80, 80, 80, 150));
    minimapTexture.setView(minimapView);
    map.drawMinimap(minimapTexture);
    
    minimapTexture.setView(minimapTexture.getDefaultView());

    sf::Vector2i pixelPos = minimapTexture.mapCoordsToPixel(player.getCenterPosition(), minimapView);
    sf::Vector2f dotPos(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

    sf::CircleShape playerDot(5.0f);
    playerDot.setFillColor(sf::Color(255, 0, 0, 150));
    playerDot.setOrigin(5.0f, 5.0f);
    playerDot.setPosition(dotPos);
    minimapTexture.draw(playerDot);

    minimapTexture.display();

    minimapSprite.setPosition(minimapPosition);
    minimapBox.setPosition(minimapPosition - PADDING);
    minimapBox.setSize(minimapSize + PADDING * 2.0f);

    sf::Vector2f scaleCorrection(1.0f, 1.0f);
    if (lastMinimapTextureSize.x > 0 && lastMinimapTextureSize.y > 0) {
        scaleCorrection.x = minimapSize.x / lastMinimapTextureSize.x;
        scaleCorrection.y = minimapSize.y / lastMinimapTextureSize.y;
    }
    minimapSprite.setScale(scaleCorrection);
}

void UI::update(const float& dt, Player& player, const sf::Vector2f& uiSize, const TileMap& map) {
    updateHealthBar(dt, player);

    updateLevelAndXP(dt, player);

    if (player.isUpdateQuest()) {
        questsBoxVisible = true;
    }
    updateQuests(dt, player.getQuests(), uiSize);

    updateMinimap(dt, player, uiSize, map);
}

void UI::draw(sf::RenderWindow& window) {
    window.draw(levelBackground);
    window.draw(levelText);

    window.draw(healthBarBackground);
    window.draw(healthBar);
    window.draw(healthPointsText);

    window.draw(XPBarBackground);
    window.draw(XPBar);
    window.draw(XPText);

    window.draw(questsBox);
    window.draw(questsBoxLabel);
    
    sf::Vector2f targetQuestsBoxSize = questsText.getLocalBounds().getSize();
    if (questsBoxSize.x >= targetQuestsBoxSize.x - PADDING.x * 2 && questsBoxSize.y >= targetQuestsBoxSize.y - PADDING.y * 2) {
        window.draw(questsText);
    }

    window.draw(minimapBox);
    window.draw(minimapSprite);
}

void UI::updateQuestsBox() {
    questsBoxVisible = !questsBoxVisible;
}

void UI::updateMinimapBoxSize(bool minimize) {
    if (minimize) {
        minimapViewRatio /= 1.2;
    }
    else {
        minimapViewRatio *= 1.2;
    }
}

void UI::openMap(const sf::Vector2f& uiSize) {    
    fullMinimapActive = !fullMinimapActive;
}