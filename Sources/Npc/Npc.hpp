#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class NPCState {
    IDLE,
    GIVING_QUEST,
    REQUIRED,
    COMPLETED_QUEST
};

class Npc {
private:
    NPCState                 state;
    int                      ID;
    std::vector<std::string> dialogue;
    sf::FloatRect            hitbox;
    
    int  dialogueIndex;
    bool finishedTalk;

public:
    Npc(int _id, const sf::FloatRect& _hitbox);

    void setRequired();

    void givingQuest();
    void completedQuest();
    void isInterruptedGivingQuest();

    std::string getDialogue();

    bool isFinishedTalk() const;

    int getID() const;
    sf::FloatRect getHitbox() const;
};