#include "FinishObjective.hpp"

void FinishObjective::updateProgress(const QuestEventData& data) {
}

bool FinishObjective::isFinished() const {
    return true;
}

std::string FinishObjective::getDescription() const {
    return std::string();
}

std::shared_ptr<QuestObjective> FinishObjective::clone() const {
    return std::make_shared<FinishObjective>(*this);
}