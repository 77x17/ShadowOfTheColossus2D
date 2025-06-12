#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

enum class QuestState {
    NOT_ACCEPTED,
    IN_PROGRESS,
    READY_TO_TURN_IN,
    COMPLETED
};

class Quest {
private:
    int         npcID;
    std::string title;
    std::string description;
    std::vector<std::shared_ptr<QuestObjective>> objectives;
    
    QuestState state;
    bool       rewardGiven;
    int        rewardExp;

public:
    Quest(int _npcID, const std::string& _title, const std::string& _description, int exp);
    
    void addObjective(const std::shared_ptr<QuestObjective>& objective);
    
    bool isCompleted() const;

    bool isFinishedObjectives() const;

    bool isReceiveReward() const;

    bool accept(const int& _npcID);
    void update(const std::string& eventType, const std::string& target);
    bool turnIn(const int& _npcID);

    std::string getQuestInformation(const int& idx) const;

    int getRewardExp();
};