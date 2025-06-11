#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

class Quest {
private:
    std::string title;
    std::string description;
    std::vector<std::shared_ptr<QuestObjective>> objectives;
    bool rewardGiven = false;
    int rewardExp;

public:
    Quest(const std::string& _title, const std::string& _description, int exp);

    bool isCompleted() const;
    
    void addObjective(const std::shared_ptr<QuestObjective>& objective);

    void update(const std::string& eventType, const std::string& target);

    void printObjectives() const;

};