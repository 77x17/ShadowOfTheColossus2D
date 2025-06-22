#include "BossAltar.hpp"


BossAltar::BossAltar(const int& m_ID, const sf::FloatRect& m_hitbox) 
: ID(m_ID), hitbox(m_hitbox) {
    // --- [Begin] - Interact text --- 
    FADE_SPEED                  = 5.0f;
    interactTextOpacity         = 0.0f;
    previousInteractTextOpacity = 0.0f;
    
    interactText.setFont(Font::font);
    interactText.setCharacterSize(12.5f);
    interactText.setOutlineThickness(2.0f);
    interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
    interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
    interactText.setString("Press [F] to interact");
    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
    interactText.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2, -hitbox.getSize().y));
    // --- [End] ---
}

void BossAltar::update(const float& dt) {
    if (interactCooldownTimer > 0) {
        interactCooldownTimer -= dt;
    }

    if (collisionWithPlayer) {
        collisionWithPlayer = false;
        interactTextOpacity += (255 - interactTextOpacity) * FADE_SPEED * dt;
    }
    else {
        interactTextOpacity += (0   - interactTextOpacity) * FADE_SPEED * dt;
    }

    if (std::abs(previousInteractTextOpacity - interactTextOpacity) > ZERO_EPSILON) {
        interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
        interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
        previousInteractTextOpacity = interactTextOpacity;
    }
}

void BossAltar::draw(sf::RenderTarget& target) const {
    sf::RectangleShape hitboxShape;
    hitboxShape.setPosition(hitbox.getPosition());
    hitboxShape.setSize(hitbox.getSize());
    hitboxShape.setOutlineColor(sf::Color::White);
    hitboxShape.setOutlineThickness(1.f);
    hitboxShape.setFillColor(sf::Color::Transparent);
    target.draw(hitboxShape);
}

void BossAltar::drawInteractText(sf::RenderTarget& target) const {
    target.draw(interactText);
}

sf::FloatRect BossAltar::getHitbox() const {
    return hitbox;
}

void BossAltar::interactWithPlayer(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (interactCooldownTimer > 0) {
        return;
    }

    std::cerr << "Vcl\n";

    interactCooldownTimer = INTERACT_COOLDOWN;
}