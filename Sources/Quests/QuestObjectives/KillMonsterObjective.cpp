#include "KillMonsterObjective.hpp"

KillMonsterObjective::KillMonsterObjective(const std::string& type, int count) {
    monsterType   = type;
    requiredCount = count;
    currentCount  = 0;
}

void KillMonsterObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "kill" && data.targetName == monsterType && !isFinished()) {
        currentCount++;
    }
}

bool KillMonsterObjective::isFinished() const {
    return currentCount >= requiredCount;
}

std::string KillMonsterObjective::getDescription() const {
    return "Defeat " + std::to_string(currentCount) + "/" + std::to_string(requiredCount) + " " + monsterType;
}

std::shared_ptr<QuestObjective> KillMonsterObjective::clone() const {
    return std::make_shared<KillMonsterObjective>(*this);
}