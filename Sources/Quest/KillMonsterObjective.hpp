#pragma once

#include "QuestObjective.hpp"

class KillMonsterObjective : public QuestObjective {
private:
    std::string monsterType;
    int requiredCount;
    int currentCount;

public:
    KillMonsterObjective(const std::string& type, int count);

    void updateProgress(const std::string& eventType, const std::string& target) override;

    bool isCompleted() const override;

    std::string getDescription() const override;
};