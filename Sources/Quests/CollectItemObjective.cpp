#include "CollectItemObjective.hpp"

CollectItemObjective::CollectItemObjective(const std::string& name, int amount) {
    itemName       = name;
    requiredAmount = amount;
    currentAmount  = 0;
}

void CollectItemObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "collectItem" && data.targetName == itemName && !isFinished()) {
        currentAmount++;
    }
}

bool CollectItemObjective::isFinished() const {
    return currentAmount >= requiredAmount;
}

std::string CollectItemObjective::getDescription() const {
    return "Collect " + std::to_string(currentAmount) + "/" + std::to_string(requiredAmount) + " " + itemName;
}

QuestEventData CollectItemObjective::getQuestEventData() const {
    QuestEventData data;
    data.eventType  = "collectItem";
    data.targetName = itemName;
    return data;
}