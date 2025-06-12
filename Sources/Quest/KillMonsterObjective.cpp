#include "KillMonsterObjective.hpp"

KillMonsterObjective::KillMonsterObjective(const std::string& type, int count) {
    monsterType   = type;
    requiredCount = count;
    currentCount  = 0;
}

void KillMonsterObjective::updateProgress(const std::string& eventType, const std::string& target) {
    if (eventType == "kill" && target == monsterType && !isFinished()) {
        currentCount++;
    }
}

bool KillMonsterObjective::isFinished() const {
    return currentCount >= requiredCount;
}

std::string KillMonsterObjective::getDescription() const {
    return "Defeat " + std::to_string(currentCount) + '/' + std::to_string(requiredCount) + ' ' + monsterType;
}