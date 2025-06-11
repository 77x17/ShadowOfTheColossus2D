#include "Quest.hpp"

Quest::Quest(const std::string& _title, const std::string& _description, int exp) {
    title       = title;
    description = description;
    rewardExp   = exp;
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

void Quest::printObjectives() const {
    std::cerr << "Quest: " << title << std::endl;
    std::cerr << description << std::endl;
    for (const auto& obj : objectives) {
        std::cerr << "- " << obj->getDescription() << (obj->isCompleted() ? " (Done)" : "") << std::endl;
    }
}