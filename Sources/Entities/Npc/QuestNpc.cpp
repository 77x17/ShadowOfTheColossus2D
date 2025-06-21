#include "QuestNpc.hpp"

#include "Font.hpp"

QuestNpc::QuestNpc(int newID, const sf::FloatRect& newHitbox, const std::string& name, const std::string& spriteName) 
: Npc(newHitbox, name, spriteName), ID(newID) {
    FADE_SPEED                  = 5.0f;
    interactTextOpacity         = 0.0f;
    previousInteractTextOpacity = 0.0f;
    
    interactText.setFont(Font::font);
    interactText.setCharacterSize(12.5f);
    interactText.setOutlineThickness(2.0f);
    interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
    interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
    interactText.setString("Press [F] to talk");
    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);
    interactText.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2, -hitbox.getSize().y));
}

void QuestNpc::update(const float& dt) {
    Npc::update(dt);

    if (collisionWithPlayer) {
        collisionWithPlayer = false;
        interactTextOpacity += (255 - interactTextOpacity) * FADE_SPEED * dt;
    }
    else {
        interactTextOpacity += (0   - interactTextOpacity) * FADE_SPEED * dt;

        if (40 < interactTextOpacity && interactTextOpacity < 50) {
            interactText.setString("Press [F] to talk");
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

void QuestNpc::drawInteractQuest(sf::RenderTarget& target) const {
    target.draw(interactText);
}

void QuestNpc::interactWithPlayer(Player& player) {
    if (interactCooldownTimer > 0) {
        return;
    }

    for (Quest& quest : player.getQuests()) {
        QuestEventData dataPack;
        dataPack.eventType = "talk";
        dataPack.npcID     = ID;
        quest.update(dataPack);

        if (ID == quest.getID()) {
            if (quest.isSuitableForGivingQuest(player.getLevel())) {
                if (quest.isCompleted()) {
                    interactText.setString("Thanks for your help!");
                    continue;
                }
                else if (quest.isFinishedDialogue()) {
                    if (quest.accept()) {
                        player.updateQuest = true;

                        quest.update(dataPack);
                    }
                    else {
                        // --- [Begin] - giveItemObjective ---
                        std::vector<std::shared_ptr<QuestObjective>> questObjectives = quest.getQuestObjectives();
                        for (auto& objective : questObjectives) if (!objective->isFinished()) {
                            QuestEventData objectiveData = objective->getQuestEventData();
                            if (objectiveData.eventType == "giveItem") {
                                for (auto& item : *player.getInventory()) if (item) {
                                    if (item->name == objectiveData.targetName) {
                                        QuestEventData giveItemData;
                                        giveItemData.eventType  = "giveItem";
                                        giveItemData.targetName = item->name;
                                        quest.update(giveItemData);
                                        
                                        item = nullptr; // take item from player

                                        if (objective->isFinished()) {
                                            break;
                                        }
                                    }
                                }
                            }
                        }

                        if (quest.isFinishObjectives()) {
                            interactText.setString("Good, that's all of them. I appreciate it");
                        }
                        else {
                            interactText.setString("I need your support");
                        }
                        // --- [End] - giveItemObjective ---

                        continue;
                    }
                }
                else {
                    interactText.setString(quest.getDialogue());
                }
            }
            else {
                interactText.setString(quest.getRequired());
            }

            break;
        }
    }

    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top  + interactText.getLocalBounds().height / 2);

    SoundManager::playSound("talk");

    interactCooldownTimer = INTERACT_COOLDOWN;
}