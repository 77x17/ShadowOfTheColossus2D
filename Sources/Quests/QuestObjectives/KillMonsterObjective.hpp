#pragma once

#include "QuestObjective.hpp"

class KillMonsterObjective : public QuestObjective {
private:
    std::string monsterType;
    int requiredCount;
    int currentCount;

public:
    KillMonsterObjective(const std::string& type, int count);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;

    std::shared_ptr<QuestObjective> clone() const override;
};