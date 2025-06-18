#include "UI.hpp"

#include "Font.hpp"
#include "Quest.hpp"
#include "Constants.hpp"
#include "Region.hpp"

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
        MINIMAP_LEAP_SPEED     = 3.0f;
        MINIMAP_SIZE           = sf::Vector2f(150, 150);
        minimapSize            = sf::Vector2f(150, 150);
        minimapViewRatio       = 6.0f;
        minimapPosition        = sf::Vector2f(WINDOW_WIDTH, 0);
        lastMinimapPosition    = sf::Vector2f(WINDOW_WIDTH, 0);

        minimapSprite.setTexture(minimapTexture.getTexture());

        minimapBox.setSize(sf::Vector2f(minimapSize) + PADDING * 2.0f);
        minimapBox.setFillColor(sf::Color(80, 80, 80, 150));
        minimapBox.setOutlineThickness(BOX_OUTLINE_THICKNESS);
        minimapBox.setOutlineColor(sf::Color::Black);

        minimapPlayerDot.setRadius(5.0f);
        minimapPlayerDot.setFillColor(sf::Color(255, 0, 0, 150));
        minimapPlayerDot.setOrigin(minimapPlayerDot.getRadius(), minimapPlayerDot.getRadius());

        fullMinimapActive = false;

        REGION_NAME_MINIMUM_SIZE = 12.5f;
        REGION_NAME_MAXIMUM_SIZE = 30.0f;
        regionNameSize           = REGION_NAME_MINIMUM_SIZE;
        regionName.setFont(Font::font);
        regionName.setCharacterSize(regionNameSize);
        regionName.setFillColor(sf::Color::White);
        regionName.setOutlineThickness(1.0f);
        regionName.setOutlineColor(sf::Color::Black);

        clockText.setFont(Font::font);
        clockText.setCharacterSize(REGION_NAME_MINIMUM_SIZE);
        clockText.setFillColor(sf::Color::White);
        clockText.setOutlineThickness(1.0f);
        clockText.setOutlineColor(sf::Color::Black);
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

void UI::updateMinimap(const float& dt, const Player& player, const sf::Vector2f& uiSize) {
    sf::Vector2f targetSize;
    sf::Vector2f targetPosition;
    float        targetTextSize;
    if (!fullMinimapActive) {
        targetPosition = sf::Vector2f(uiSize.x - minimapSize.x - MINIMAP_PADDING.x, MINIMAP_PADDING.y);
        targetSize     = MINIMAP_SIZE;
        minimapSprite.setColor(sf::Color(255, 255, 255, 150));
        targetTextSize = REGION_NAME_MINIMUM_SIZE;
    }
    else {
        targetPosition = MINIMAP_PADDING;
        targetSize     = uiSize - MINIMAP_PADDING * 2.0f;
        minimapSprite.setColor(sf::Color(255, 255, 255, 250)); 
        targetTextSize = REGION_NAME_MAXIMUM_SIZE;
    }

    minimapSize     += (targetSize     - minimapSize)     * MINIMAP_LEAP_SPEED * dt;
    minimapPosition += (targetPosition - minimapPosition) * MINIMAP_LEAP_SPEED * dt;
    regionNameSize  += (targetTextSize - regionNameSize)  * MINIMAP_LEAP_SPEED * dt;

    sf::Vector2u mapPixelSize = minimapTexture.getSize();
    sf::Vector2f playerPos = player.getCenterPosition(); // world coordinates
    sf::Vector2f viewSize(minimapSize * minimapViewRatio);
    sf::IntRect viewRect;
    viewRect.width  = std::min(static_cast<int>(viewSize.x), static_cast<int>(mapPixelSize.x));
    viewRect.height = std::min(static_cast<int>(viewSize.y), static_cast<int>(mapPixelSize.y));

    viewRect.left   = static_cast<int>(playerPos.x - viewRect.width / 2.f);
    viewRect.top    = static_cast<int>(playerPos.y - viewRect.height / 2.f);
    if (viewRect.left < 0) viewRect.left = 0;
    if (viewRect.left + viewRect.width > static_cast<int>(mapPixelSize.x)) {
        viewRect.left = mapPixelSize.x - viewRect.width;
    }
    if (viewRect.top < 0) viewRect.top = 0;
    if (viewRect.top + viewRect.height > static_cast<int>(mapPixelSize.y)) {
        viewRect.top = mapPixelSize.y - viewRect.height;
    }

    minimapSprite.setTexture(minimapTexture.getTexture());
    minimapSprite.setTextureRect(viewRect);

    if (viewRect.width == 0 || viewRect.height == 0) {
        std::cerr << "[Bug] - UI.cpp - updateMinimap()\n";
    }
    sf::Vector2f scale = sf::Vector2f(minimapSize.x / viewRect.width, minimapSize.y / viewRect.height);
    minimapSprite.setScale(scale);

    minimapSprite.setPosition(minimapPosition);
    minimapBox.setPosition(minimapPosition - PADDING);
    minimapBox.setSize(sf::Vector2f(minimapSize) + PADDING * 2.0f);

    sf::Vector2f localPlayerPos(
        playerPos.x - static_cast<float>(viewRect.left),
        playerPos.y - static_cast<float>(viewRect.top)
    );

    // std::cerr << localPlayerPos.x << ' ' << localPlayerPos.y << '\n';

    sf::Vector2f scaledPlayerPos(
        localPlayerPos.x * scale.x,
        localPlayerPos.y * scale.y
    );

    sf::Vector2f dotPosition = minimapPosition + scaledPlayerPos;

    minimapPlayerDot.setPosition(dotPosition);

    regionName.setCharacterSize(regionNameSize);
    regionName.setString(Region::getName(player.getCollisionRegionID()));
    regionName.setOrigin(regionName.getLocalBounds().left + regionName.getLocalBounds().width / 2, 0);
                        //  regionName.getLocalBounds().top + regionName.getLocalBounds().height / 2);
    regionName.setPosition(minimapPosition + sf::Vector2f(minimapSize.x / 2, PADDING.y * 2));
}

void UI::updateClock(const sf::Vector2f& uiSize, const Clock& gameClock) {    
    clockText.setString(gameClock.getTimeString() + " - " + gameClock.getTimeOfDay());
    clockText.setOrigin(clockText.getLocalBounds().left + clockText.getLocalBounds().width / 2.0, 0);

    clockText.setPosition(minimapPosition + sf::Vector2f(minimapSize.x / 2, minimapSize.y + PADDING.y * 2));
}

void UI::update(const float& dt, Player& player, const sf::Vector2f& uiSize, const Clock& gameClock) {
    updateHealthBar(dt, player);

    updateLevelAndXP(dt, player);

    if (player.isUpdateQuest()) {
        questsBoxVisible = true;
    }
    updateQuests(dt, player.getQuests(), uiSize);

    updateMinimap(dt, player, uiSize);

    updateClock(uiSize, gameClock);
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
    window.draw(minimapPlayerDot);
    window.draw(regionName);
    window.draw(clockText);
}

void UI::updateQuestsBox() {
    questsBoxVisible = !questsBoxVisible;

    if (questsBoxVisible) {
        SoundManager::playSound("menuOpen");
    }
    else {
        SoundManager::playSound("menuClose");
    }
}

void UI::generateMinimapTexture(const TileMap& map) {
    sf::Vector2u mapSize = map.getPixelSize();

    minimapTexture.create(mapSize.x, mapSize.y);
    minimapTexture.clear(sf::Color::Transparent);
    
    map.drawMinimap(minimapTexture);
    
    minimapTexture.setSmooth(false);
    minimapTexture.display();
}

void UI::updateMinimapBoxSize(bool minimize) {
    if (minimize) {
        minimapViewRatio /= 1.2;
    }
    else {
        minimapViewRatio *= 1.2;
    }

    if (minimapViewRatio >= 8) {
        minimapViewRatio = 8;
    }
    else if (minimapViewRatio <= 0.75) {
        minimapViewRatio = 0.75;
    }

    // std::cerr << minimapViewRatio << '\n';
}

void UI::openMap(const sf::Vector2f& uiSize) {
    fullMinimapActive = !fullMinimapActive;

    if (fullMinimapActive) {
        SoundManager::playSound("menuOpen");
    }
    else {
        SoundManager::playSound("menuClose");
    }
}