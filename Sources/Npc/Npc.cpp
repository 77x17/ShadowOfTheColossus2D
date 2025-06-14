#include "Npc.hpp"

#include <iostream>

Npc::Npc(int _ID, const sf::FloatRect& _hitbox) {
    state  = NPCState::IDLE;
    ID     = _ID;
    hitbox = _hitbox;

    dialogueIndex  = 0;
    finishedTalk   = false;
}

void Npc::setRequired() {
    state = NPCState::REQUIRED;
}

void Npc::givingQuest() {
    if (state == NPCState::IDLE) {
        state = NPCState::GIVING_QUEST;
    }
}

void Npc::completedQuest() {
    if (state == NPCState::GIVING_QUEST) {
        state = NPCState::COMPLETED_QUEST;
    }
}

void Npc::isInterruptedGivingQuest() {
    if ((state == NPCState::GIVING_QUEST || state == NPCState::REQUIRED) && !finishedTalk) {
        state          = NPCState::IDLE;
        dialogueIndex  = 0;
    }
}

std::string Npc::getDialogue() {
    switch (state) {
        case NPCState::IDLE: {
            return "Hello";
        }
        case NPCState::GIVING_QUEST: {
            if (dialogueIndex < (int)dialogue.size()) {
                return dialogue[dialogueIndex++];
            }
            else {
                finishedTalk = true; 
                return "I need your support";
            }
        }
        case NPCState::REQUIRED: {
            return "You must be at least Lv." + std::to_string(1) + "\nto accept this quest";
        }
        case NPCState::COMPLETED_QUEST: {
            return "I'm very appreciated";
        }
        default: {
            std::cerr << "[Bug] - Npc.cpp - getDialogue()\n";
            return "[Bug] - Npc.cpp - getDialogue()";
        }
    }
}

bool Npc::isFinishedTalk() const {
    return finishedTalk == true;
}

int Npc::getID() const {
    return ID;
}

sf::FloatRect Npc::getHitbox() const {
    return hitbox;
}