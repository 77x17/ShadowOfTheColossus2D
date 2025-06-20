#include "GiveItemObjective.hpp"

GiveItemObjective::GiveItemObjective(const std::string& name, int amount, int _npcID) {
    itemName         = name;
    requiredAmount   = amount;
    npcID            = _npcID;
    finishedGiveItem = true;
}

void GiveItemObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "finishedGiveItem" && data.targetName == itemName && !isFinished()) {
        finishedGiveItem = true;
    }
}

bool GiveItemObjective::isFinished() const {
    return finishedGiveItem == true;
}

std::string GiveItemObjective::getDescription() const {
    return "Collect " + std::to_string(requiredAmount) + " " + itemName;
}

QuestEventData GiveItemObjective::getQuestEventData() const {
    QuestEventData data;
    data.eventType  = "giveItem";
    data.targetName = itemName;
    data.amount     = requiredAmount;
    return data;

}