#include "Quest.hpp"

Quest::Quest(int _npcID, const std::string& _title, const std::string& _description, int exp) {
    npcID       = _npcID;
    title       = _title;
    description = _description;
    state       = QuestState::NOT_ACCEPTED;
    rewardExp   = exp;
    rewardGiven = false;
}

bool Quest::isCompleted() const {
    return state == QuestState::COMPLETED;
}

bool Quest::isFinishedObjectives() const {
    for (const std::shared_ptr<QuestObjective>& objective : objectives) {
        if (!objective->isFinished()) {
            return false;
        }
    }

    return true;
}

bool Quest::isReceiveReward() const {
    return rewardGiven;
}

void Quest::addObjective(const std::shared_ptr<QuestObjective>& objective) {
    objectives.push_back(objective);
}

bool Quest::accept(const int& _npcID) {
    if (state == QuestState::NOT_ACCEPTED && _npcID == npcID) {
        state = QuestState::IN_PROGRESS;

        //std::cerr << "Quest accepted: '" << title << "'\n";

        return true;
    }

    return false;
}

void Quest::update(const std::string& eventType, const std::string& target) {
    if (state != QuestState::IN_PROGRESS) {
        return;
    }

    for (std::shared_ptr<QuestObjective>& objective : objectives) {
        objective->updateProgress(eventType, target);
    }

    if (isFinishedObjectives()) {
        state = QuestState::READY_TO_TURN_IN;
        
        //std::cerr << "Quest ready to turn in: '" << title << "'\n";
    }
}

bool Quest::turnIn(const int& _npcID) {
    if (state == QuestState::READY_TO_TURN_IN && _npcID == npcID) {
        state = QuestState::COMPLETED;

        //std::cerr << "Quest turned in: '" << title << "'\n";
        
        return true;
    }

    return false;
}

std::string Quest::getQuestInformation(const int& idx) const {
    std::string display = "[" + std::to_string(idx) + "] " + title + " ";

    switch (state) {
        case QuestState::NOT_ACCEPTED: {
            display += "[Not accepted]\n";

            break;
        }
        case QuestState::IN_PROGRESS: {
            display += "[In progress]\n";
            display += description + '\n';
            for (const std::shared_ptr<QuestObjective>& objective : objectives) {
                display += "- " + objective->getDescription() + (objective->isFinished() ? " (Done)" : "") + '\n';
            }

            break;
        }
        case QuestState::READY_TO_TURN_IN: {
            display += "[Ready to turn in]\n";

            break;
        }
        case QuestState::COMPLETED: {
            display += "[Completed]\n";

            break;
        }
    }

    return display;
}

int Quest::getRewardExp() {
    if (state != QuestState::COMPLETED) {
        return 0;
    }

    rewardGiven = true;

    return rewardExp;
}