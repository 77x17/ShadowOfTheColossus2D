#include "NpcManager.hpp"

void NpcManager::loadNpcs(const TileMap& map) {
    npcs.push_back(std::make_unique<QuestNpc>(
        0,
        map.getQuestNpcRects().at(0),
        "Elder Thorne",
        "npc_00"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        1,
        map.getQuestNpcRects().at(1),
        "Torren",
        "npc_01"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        2,
        map.getQuestNpcRects().at(2),
        "Mira",
        "npc_02"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        3,
        map.getQuestNpcRects().at(3),
        "Bren",
        "npc_03"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        4,
        map.getQuestNpcRects().at(4),
        "Elara",
        "npc_02"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        5,
        map.getQuestNpcRects().at(5),
        "???",
        "npc_02"
    ));

    for (const sf::FloatRect& merchantRect : map.getMerchantNpcRects()) {    
        npcs.push_back(std::make_unique<MerchantNpc>(
            merchantRect,
            "Merchant",
            "npc_00"
        ));
    }
    
}

void NpcManager::handlePlayerInteraction(Player& player) {
    for (const std::unique_ptr<Npc>& npc : npcs) {
        if (player.isCollision(npc->getHitbox())) {
            player.collisionWithNpc  = true;
            npc->collisionWithPlayer = true;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                npc->interactWithPlayer(player);
            }
        }
    }
}

void NpcManager::update(const float& dt) {
    for (std::unique_ptr<Npc>& npc : npcs) {
        npc->update(dt);
    }
}

void NpcManager::draw(sf::RenderTarget& target) const {
    for (const std::unique_ptr<Npc>& npc : npcs) {
        npc->draw(target);
    }
}

void NpcManager::drawInteractText(sf::RenderTarget& target) const {
    for (const std::unique_ptr<Npc>& npc : npcs) {
        npc->drawInteractText(target);
    }
}

bool NpcManager::isInteractWithMerchant(bool inventoryVisible) {
    bool merchantFlag = false;
    for (std::unique_ptr<Npc>& npc : npcs) {    
        if (MerchantNpc* merchantNpc = dynamic_cast<MerchantNpc*>(npc.get())) {
            if (merchantNpc->isInteractWithPlayer()) {
                merchantFlag = true;

                if (inventoryVisible) {
                    merchantFlag = false;
                    merchantNpc->interruptedInteract();
                }
            }
        }
    }
    return merchantFlag;
}