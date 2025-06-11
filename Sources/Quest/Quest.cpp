#include "Quest.hpp"

Quest::Quest(const std::string& _title, const std::string& _description, int exp) {
    title       = _title;
    description = _description;
    rewardExp   = exp;
    rewardGiven = false;
}

bool Quest::isCompleted() const {
    for (const std::shared_ptr<QuestObjective>& objective : objectives) {
        if (!objective->isCompleted()) {
            return false;
        }
    }

    return true;
}

void Quest::addObjective(const std::shared_ptr<QuestObjective>& objective) {
    objectives.push_back(objective);
}

void Quest::update(const std::string& eventType, const std::string& target) {
    for (std::shared_ptr<QuestObjective>& objective : objectives) {
        objective->updateProgress(eventType, target);
    }

    if (isCompleted() && !rewardGiven) {
        rewardGiven = true;
        std::cerr << "Quest '" << title << "' completed! You earned " << rewardExp << " gold!" << '\n';
    }
}

std::string Quest::getObjectives(const int& idx) const {
    std::string display = std::string();
    display += "[" + std::to_string(idx) + "] " + title + '\n';
    display += description + '\n';
    for (const std::shared_ptr<QuestObjective>& objective : objectives) {
        display += "- " + objective->getDescription() + (objective->isCompleted() ? " (Done)" : "") + '\n';
    }
    return display;
}