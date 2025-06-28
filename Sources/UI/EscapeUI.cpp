#include "EscapeUI.hpp"

#include "SoundManager.hpp"
#include "Font.hpp"

EscapeUI::EscapeUI(const sf::Vector2f& size) {
    saveBox  .setFillColor(sf::Color(0, 0, 0, 175));
    loadBox  .setFillColor(sf::Color(0, 0, 0, 175));
    configBox.setFillColor(sf::Color(0, 0, 0, 175));
    quitBox  .setFillColor(sf::Color(0, 0, 0, 175));
    saveBox  .setOutlineColor(sf::Color(0, 0, 0, 255));
    loadBox  .setOutlineColor(sf::Color(0, 0, 0, 255));
    configBox.setOutlineColor(sf::Color(0, 0, 0, 255));
    quitBox  .setOutlineColor(sf::Color(0, 0, 0, 255));
    saveBox  .setOutlineThickness(2.0f);
    loadBox  .setOutlineThickness(2.0f);
    configBox.setOutlineThickness(2.0f);
    quitBox  .setOutlineThickness(2.0f);

    saveText  .setFont(Font::font);
    loadText  .setFont(Font::font);
    configText.setFont(Font::font);
    quitText  .setFont(Font::font);
    saveText  .setString("SAVE");
    loadText  .setString("LOAD");
    configText.setString("CONFIG");
    quitText  .setString("QUIT");
    
    footerBox.setFillColor(sf::Color(0, 0, 0, 200));
    footerText.setFont(Font::font);
    footerText.setString("(c) 2025 77x17 - Project_H (C++, SFML)");

    updatePosition(size);
}

void EscapeUI::updatePosition(const sf::Vector2f& size) {
    containBox.setSize(size);

    sf::Vector2f boxSize       = sf::Vector2f(3 * size.x / 8.0f, 1 * size.y / 10.0f);
    sf::Vector2f startPosition = sf::Vector2f(0.0f, 2 * size.y / 10.0f);
    sf::Vector2f padding       = sf::Vector2f(0.0f, boxSize.y + size.y / 15.0f);

    saveBox  .setSize(boxSize);
    loadBox  .setSize(boxSize);
    configBox.setSize(boxSize);
    quitBox  .setSize(boxSize);
    
    saveBox  .setPosition(startPosition);
    loadBox  .setPosition(startPosition + padding);
    configBox.setPosition(startPosition + 2.0f * padding);
    quitBox  .setPosition(startPosition + 3.0f * padding);

    baseBoxPosition = -size.x / 8.0f;
    saveText  .setCharacterSize(size.y / 20.0f);
    loadText  .setCharacterSize(size.y / 20.0f);
    configText.setCharacterSize(size.y / 20.0f);
    quitText  .setCharacterSize(size.y / 20.0f);
    saveText  .setOrigin(saveText  .getLocalBounds().getPosition() + saveText  .getLocalBounds().getSize() / 2.0f);
    loadText  .setOrigin(loadText  .getLocalBounds().getPosition() + loadText  .getLocalBounds().getSize() / 2.0f);
    configText.setOrigin(configText.getLocalBounds().getPosition() + configText.getLocalBounds().getSize() / 2.0f);
    quitText  .setOrigin(quitText  .getLocalBounds().getPosition() + quitText  .getLocalBounds().getSize() / 2.0f);
    saveText  .setPosition(saveBox  .getGlobalBounds().getPosition() + sf::Vector2f(saveBox  .getGlobalBounds().getSize().x, saveBox  .getGlobalBounds().getSize().y / 2.0f));
    loadText  .setPosition(loadBox  .getGlobalBounds().getPosition() + sf::Vector2f(loadBox  .getGlobalBounds().getSize().x, loadBox  .getGlobalBounds().getSize().y / 2.0f));
    configText.setPosition(configBox.getGlobalBounds().getPosition() + sf::Vector2f(configBox.getGlobalBounds().getSize().x, configBox.getGlobalBounds().getSize().y / 2.0f));
    quitText  .setPosition(quitBox  .getGlobalBounds().getPosition() + sf::Vector2f(quitBox  .getGlobalBounds().getSize().x, quitBox  .getGlobalBounds().getSize().y / 2.0f));

    footerBox.setSize(sf::Vector2f(size.x, boxSize.y / 3.0f));
    footerBox.setPosition(sf::Vector2f(0.0f, size.y - footerBox.getGlobalBounds().getSize().y));
    footerText.setCharacterSize(footerBox.getGlobalBounds().getSize().y / 2.0f);
    footerText.setOrigin(footerText.getLocalBounds().getPosition() + footerText.getLocalBounds().getSize() / 2.0f);
    footerText.setPosition(footerBox.getGlobalBounds().getPosition() + footerBox.getGlobalBounds().getSize() / 2.0f);
}

void EscapeUI::toggle() {
    escapeUiActive = !escapeUiActive;

    if (escapeUiActive) {
        SoundManager::playSound("menuOpen");

        targetOpacity = 150.0f;
    }
    else {
        SoundManager::playSound("menuClose");
        
        targetOpacity = 0.0f;
    }
}

void EscapeUI::update(const float& dt, const sf::Vector2f& mousePosition) {
    currentOpacity += (targetOpacity - currentOpacity) * OPACITY_LEAP_SPEED * dt;
    containBox.setFillColor(sf::Color(0, 0, 0, currentOpacity));
    
    if (currentOpacity >= 140.0f) {
        if (saveBox.getGlobalBounds().contains(mousePosition)) {
            if (select != 0) SoundManager::playSound("menutap");
            select = 0;
        }
        else if (loadBox.getGlobalBounds().contains(mousePosition)) {
            if (select != 1) SoundManager::playSound("menutap");
            select = 1;
        }
        else if (configBox.getGlobalBounds().contains(mousePosition)) {
            if (select != 2) SoundManager::playSound("menutap");
            select = 2;
        }
        else if (quitBox.getGlobalBounds().contains(mousePosition)) {
            if (select != 3) SoundManager::playSound("menutap");
            select = 3;
        }
        else {
            select = -1;
        }

        currentSaveBoxPosition   =   saveBox.getPosition().x;
        currentLoadBoxPosition   =   loadBox.getPosition().x;
        currentConfigBoxPosition = configBox.getPosition().x;
        currentQuitBoxPosition   =   quitBox.getPosition().x;

        currentSaveBoxPosition   += ((select == 0 ? targetSelectPosition : baseBoxPosition) - currentSaveBoxPosition  ) * SELECT_LEAP_SPEED * dt;
        currentLoadBoxPosition   += ((select == 1 ? targetSelectPosition : baseBoxPosition) - currentLoadBoxPosition  ) * SELECT_LEAP_SPEED * dt;
        currentConfigBoxPosition += ((select == 2 ? targetSelectPosition : baseBoxPosition) - currentConfigBoxPosition) * SELECT_LEAP_SPEED * dt;
        currentQuitBoxPosition   += ((select == 3 ? targetSelectPosition : baseBoxPosition) - currentQuitBoxPosition  ) * SELECT_LEAP_SPEED * dt;

        saveBox  .setPosition(sf::Vector2f(currentSaveBoxPosition  , saveBox  .getPosition().y));
        loadBox  .setPosition(sf::Vector2f(currentLoadBoxPosition  , loadBox  .getPosition().y));
        configBox.setPosition(sf::Vector2f(currentConfigBoxPosition, configBox.getPosition().y));
        quitBox  .setPosition(sf::Vector2f(currentQuitBoxPosition  , quitBox  .getPosition().y));

        saveText  .setFillColor(select == 0 ? sf::Color::Yellow : sf::Color::White);
        loadText  .setFillColor(select == 1 ? sf::Color::Yellow : sf::Color::White);
        configText.setFillColor(select == 2 ? sf::Color::Yellow : sf::Color::White);
        quitText  .setFillColor(select == 3 ? sf::Color::Yellow : sf::Color::White);
        saveText  .setPosition(saveBox  .getGlobalBounds().getPosition() + sf::Vector2f(3 * saveBox  .getGlobalBounds().getSize().x / 4.0f, saveBox  .getGlobalBounds().getSize().y / 2.0f));
        loadText  .setPosition(loadBox  .getGlobalBounds().getPosition() + sf::Vector2f(3 * loadBox  .getGlobalBounds().getSize().x / 4.0f, loadBox  .getGlobalBounds().getSize().y / 2.0f));
        configText.setPosition(configBox.getGlobalBounds().getPosition() + sf::Vector2f(3 * configBox.getGlobalBounds().getSize().x / 4.0f, configBox.getGlobalBounds().getSize().y / 2.0f));
        quitText  .setPosition(quitBox  .getGlobalBounds().getPosition() + sf::Vector2f(3 * quitBox  .getGlobalBounds().getSize().x / 4.0f, quitBox  .getGlobalBounds().getSize().y / 2.0f));
    }
}

bool EscapeUI::isVisible() const {
    return escapeUiActive;
}

void EscapeUI::draw(sf::RenderTarget& target) const {
    target.draw(containBox);

    if (currentOpacity >= 140.0f) {
        target.draw(saveBox);
        target.draw(loadBox);
        target.draw(configBox);
        target.draw(quitBox);

        target.draw(saveText);
        target.draw(loadText);
        target.draw(configText);
        target.draw(quitText);

        target.draw(footerBox);
        target.draw(footerText);
    }
}

bool EscapeUI::shouldDraw() const {
    return (isVisible() || currentOpacity > 10.0f);
}

int EscapeUI::handleClick(const sf::Vector2f& mousePosition) {
    if (currentOpacity >= 140.0f) {
        if (saveBox.getGlobalBounds().contains(mousePosition)) {
            SoundManager::playSound("talk");
            select = 0;
        }
        else if (loadBox.getGlobalBounds().contains(mousePosition)) {
            SoundManager::playSound("talk");
            select = 1;
        } 
        else if (configBox.getGlobalBounds().contains(mousePosition)) {
            SoundManager::playSound("talk");
            select = 2;
        }
        else if (quitBox.getGlobalBounds().contains(mousePosition)) {
            SoundManager::playSound("talk");
            select = 3;
        }
        else {
            select = -1;
        }
    }
    return select;
}