#pragma once

#include "QuestObjective.hpp"

class CollectItemObjective : public QuestObjective {
private:
    std::string itemName;
    int         requiredAmount;
    int         currentAmount;

public:
    CollectItemObjective(const std::string& name, int amount);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;

    QuestEventData getQuestEventData() const override;
};