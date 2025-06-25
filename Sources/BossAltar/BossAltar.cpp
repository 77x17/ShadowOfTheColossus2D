#include "BossAltar.hpp"

#include "Player.hpp"
#include "EnemyManager.hpp"

#include "Constants.hpp"
#include "Font.hpp"
#include "ItemDatabase.hpp"

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

    // --- [Begin] - Required Items ---
    switch (ID) {
        case 0: {
            requiredItems.emplace_back("Bat Orb", 1);
            break;
        }
        case 1: {
            requiredItems.emplace_back("Eye Orb", 1);
            break;
        }
        default: {
            break;
        }
    }
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

        if (40 < interactTextOpacity && interactTextOpacity < 50) {
            interactText.setString("Press [F] to interact");
            interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                                   interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
        }
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

void BossAltar::interactWithPlayer(Player& player) {
    if (interactCooldownTimer > 0) {
        return;
    }

    std::unordered_map<std::string, int> itemCounts;
    for (const std::shared_ptr<ItemData>& item : *player.getInventory()) if (item) {
        itemCounts[item->name]++;
    }

    suitableForSummonBoss = true;
    for (const auto& pair : requiredItems) {
        if (itemCounts[pair.first] < pair.second) {
            suitableForSummonBoss = false;
        }
    }
    if (suitableForSummonBoss) {
        for (const auto& pair : requiredItems) {
            int removeAmount = pair.second;
            for (std::shared_ptr<ItemData>& item : *player.getInventory()) {
                if (removeAmount <= 0) {
                    break;
                }

                if (item && item->name == pair.first) {
                    if (item->amount <= removeAmount) {
                        removeAmount -= item->amount;

                        item = nullptr;
                    }
                    else {
                        item->amount -= removeAmount;
                        removeAmount = 0;
                    }
                }
            }
        }
    }
    else {
        switch (ID) {
            case 0: {
                interactText.setString("Bat Boss Lv.5\n01 Bat Orb");
                interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width  / 2, 
                                       interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
                break;
            }
            case 1: {
                interactText.setString("Gazer Lord Lv.10\n01 Eye Orb");
                interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width  / 2, 
                                       interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
                break;
            }
            default: {
                break;
            }
        }
    }

    interactCooldownTimer = INTERACT_COOLDOWN;
}

bool BossAltar::isSuitableForSummonBoss() const {
    return suitableForSummonBoss == true;
}

void BossAltar::summonBoss(EnemyManager& enemies) {
    suitableForSummonBoss = false;

    switch (ID) {
        case 0: {
            std::vector<std::pair<float, std::shared_ptr<ItemData>>> batLordInventory;
            batLordInventory.emplace_back(0.2f, ItemDatabase::get("Bat Bow"));
            batLordInventory.emplace_back(0.2f, ItemDatabase::get("Bat Helmet"));
            batLordInventory.emplace_back(0.2f, ItemDatabase::get("Bat Chestplate"));
            batLordInventory.emplace_back(0.2f, ItemDatabase::get("Bat Leggings"));
            batLordInventory.emplace_back(0.2f, ItemDatabase::get("Bat Boots"));

            enemies.summonEnemy(std::make_unique<BatBoss>(sf::Vector2f(hitbox.getPosition()), "Bat Boss Lv.5", 100.0f, batLordInventory));
            
            break;
        }
        case 1: {
            std::vector<std::pair<float, std::shared_ptr<ItemData>>> gazerBossInventory;
            gazerBossInventory.emplace_back(0.2f, ItemDatabase::get("Bat Bow"));
            gazerBossInventory.emplace_back(0.2f, ItemDatabase::get("Bat Helmet"));
            gazerBossInventory.emplace_back(0.2f, ItemDatabase::get("Bat Chestplate"));
            gazerBossInventory.emplace_back(0.2f, ItemDatabase::get("Bat Leggings"));
            gazerBossInventory.emplace_back(0.2f, ItemDatabase::get("Bat Boots"));

            enemies.summonEnemy(std::make_unique<BatBoss>(sf::Vector2f(hitbox.getPosition()), "Gazer Lord Lv.10", 500.0f, gazerBossInventory));
            
            break;
        }
        default: {
            break;
        }
    }
}