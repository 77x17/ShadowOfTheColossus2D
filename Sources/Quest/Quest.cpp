#include "Quest.hpp"

Quest::Quest(const std::string& _title, int exp) {
    title = _title;
    npcIDs.clear();
    dialogues.clear();
    descriptions.clear();
    objectives.clear();
    requiredLevel = 0;

    state       = QuestState::NOT_ACCEPTED;
    rewardExp   = exp;
    rewardGiven = false;

    stage              = 0;
    dialogueIndex      = 0;
    turnInNotification = false;
}

void Quest::addRequiredLevel(int level) {
    requiredLevel = level;
}

void Quest::addNpcID(int _stage, int ID) {
    npcIDs.push_back(ID);
}

void Quest::addDialogue(int _stage, const std::string& dialogue, bool create) {
    if (create) {
        dialogues.push_back(std::vector<std::string>());
    }
    
    dialogues[_stage].push_back(dialogue);
}

void Quest::addDescription(int _stage, const std::string& description) {
    descriptions.push_back(description);
}

void Quest::addObjective(int _stage, const std::shared_ptr<QuestObjective>& objective, bool create) {
    if (create) {
        objectives.push_back(std::vector<std::shared_ptr<QuestObjective>>());
    }

    objectives[_stage].push_back(objective);
}


bool Quest::isSuitableForGivingQuest(int playerLevel) {
    return playerLevel >= requiredLevel;
}

bool Quest::isCompleted() const {
    return state == QuestState::COMPLETED;
}

bool Quest::isFinishedObjectives() const {
    for (const std::shared_ptr<QuestObjective>& objective : objectives[stage]) {
        if (!objective->isFinished()) {
            return false;
        }
    }

    return true;
}

bool Quest::isReceiveReward() const {
    return rewardGiven;
}

bool Quest::isFinishedDialogue() const {
    return state != QuestState::NOT_ACCEPTED || dialogueIndex >= static_cast<int>(dialogues[stage].size());
}

bool Quest::isReadyToTurnIn() {
    if (turnInNotification) {
        turnInNotification = false;
        return true;
    }
    
    return false;
}

bool Quest::accept() {
    if (state == QuestState::NOT_ACCEPTED) {
        state = QuestState::IN_PROGRESS;

        std::cerr << "Quest accepted: '" << title << "' " << "stage: " << stage << "\n";

        return true;
    }

    return false;
}

void Quest::update(const std::string& eventType, const std::string& target) {
    if (state != QuestState::IN_PROGRESS) {
        return;
    }

    for (std::shared_ptr<QuestObjective>& objective : objectives[stage]) {
        objective->updateProgress(eventType, target);
    }

    if (state == QuestState::IN_PROGRESS && isFinishedObjectives()) {
        state = QuestState::READY_TO_TURN_IN;
        turnInNotification = true;
        std::cerr << "Quest ready to turn in: '" << title << "' " << "stage: " << stage << "\n";
    }
}

bool Quest::turnIn() {
    if (state == QuestState::READY_TO_TURN_IN) {
        std::cerr << "Quest turned in: '" << title << "' " << "stage: " << stage << "\n";

        stage++;
        if (stage == static_cast<int>(npcIDs.size())) {
            state = QuestState::COMPLETED;
        }
        else {
            state = QuestState::NOT_ACCEPTED;
        }
        
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
            display += descriptions[stage] + '\n';
            for (const std::shared_ptr<QuestObjective>& objective : objectives[stage]) {
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

int Quest::getID() const {
    if (stage >= static_cast<int>(npcIDs.size())) {
        return npcIDs[0];
    }

    return npcIDs[stage];
}

std::string Quest::getDialogue() {
    if (dialogueIndex < static_cast<int>(dialogues[stage].size())) {
        return dialogues[stage][dialogueIndex++];
    }
    else {
        return "I need your support";
    }
}

std::string Quest::getRequired() const {
    return "You must be at least Lv." + std::to_string(requiredLevel) + "\nto accept this quest";
}

void Quest::isInterruptedGivingQuest() {
    dialogueIndex = 0;
}