#pragma once

#include "QuestObjective.hpp"

class GiveItemObjective : public QuestObjective {
private:
    std::string itemName;
    int  requiredAmount;
    int  npcID;
    bool finishedGiveItem;

public:
    GiveItemObjective(const std::string& name, int amount, int npcID);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;

    QuestEventData getQuestEventData() const override;
};