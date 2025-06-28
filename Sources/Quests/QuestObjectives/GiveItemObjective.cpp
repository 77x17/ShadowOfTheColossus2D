#include "GiveItemObjective.hpp"

GiveItemObjective::GiveItemObjective(const std::string& name, int amount) {
    itemName       = name;
    requiredAmount = amount;
    currentAmount  = 0;
}

void GiveItemObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "giveItem" && data.targetName == itemName && !isFinished()) {
        currentAmount += data.amount;
    }
}

bool GiveItemObjective::isFinished() const {
    return currentAmount >= requiredAmount;
}

std::string GiveItemObjective::getDescription() const {
    return "Give " + std::to_string(currentAmount) + "/" + std::to_string(requiredAmount) + " " + itemName;
}

QuestEventData GiveItemObjective::getQuestEventData() const {
    QuestEventData data;
    data.eventType  = "giveItem";
    data.targetName = itemName;
    data.amount     = requiredAmount - currentAmount;
    return data;
}

std::shared_ptr<QuestObjective> GiveItemObjective::clone() const {
    return std::make_shared<GiveItemObjective>(*this);
}