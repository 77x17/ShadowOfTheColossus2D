#include "FinishedObjective.hpp"

void FinishedObjective::updateProgress(const QuestEventData& data) {
}

bool FinishedObjective::isFinished() const {
    return true;
}

std::string FinishedObjective::getDescription() const {
    return std::string();
}