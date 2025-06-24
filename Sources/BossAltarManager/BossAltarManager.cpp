#include "BossAltarManager.hpp"

#include "Player.hpp"
#include "EnemyManager.hpp"

void BossAltarManager::loadBossAltars(const std::unordered_map<int, sf::FloatRect>& bossAltarRects) {
    bossAltars.emplace_back(
        0,
        bossAltarRects.at(0)
    );
}

void BossAltarManager::handlePlayerInteraction(Player& player) {
    for (BossAltar& bossAltar : bossAltars) {
        if (player.isCollision(bossAltar.getHitbox())) {
            bossAltar.collisionWithPlayer = true;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                bossAltar.interactWithPlayer(player);
            }
        }
    }
}

void BossAltarManager::update(const float& dt, EnemyManager& enemies) {
    for (BossAltar& bossAltar : bossAltars) {
        bossAltar.update(dt);
        if (bossAltar.isSuitableForSummonBoss()) {
            bossAltar.summonBoss(enemies);
        }
    }
}

void BossAltarManager::drawInteractText(sf::RenderTarget& target) const {
    for (const BossAltar& bossAltar : bossAltars) {
        bossAltar.drawInteractText(target);
    }
}