#include "FinishObjective.hpp"

void FinishObjective::updateProgress(const QuestEventData& data) {
}

bool FinishObjective::isFinished() const {
    return true;
}

std::string FinishObjective::getDescription() const {
    return std::string();
}