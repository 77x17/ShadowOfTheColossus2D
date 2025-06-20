#include "Quest.hpp"

#include "SoundManager.hpp"

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

    stage         = 0;
    dialogueIndex = 0;
}

void Quest::addRequiredLevel(int level) {
    requiredLevel = level;
}

void Quest::addRequiredDescription(const std::string& description) {
    requiredDescription = description;
}

void Quest::addNpcID(int _stage, int ID) {
    npcIDs.push_back(ID);
}

void Quest::addDialogue(int _stage, const std::string& dialogue) {
    if (_stage >= static_cast<int>(dialogues.size())) {
        dialogues.push_back(std::vector<std::string>());
    }
    
    if (dialogue != std::string()) {
        dialogues[_stage].push_back(dialogue);
    }
}

void Quest::addDescription(int _stage, const std::string& description) {
    descriptions.push_back(description);
}

void Quest::addObjective(int _stage, const std::shared_ptr<QuestObjective>& objective) {
    if (_stage >= static_cast<int>(objectives.size())) {
        objectives.push_back(std::vector<std::shared_ptr<QuestObjective>>());
    }

    objectives[_stage].push_back(objective);
}

void Quest::addItemFromNpc(int _stage, const std::shared_ptr<ItemData>& item) {
    itemFromNpc.emplace_back(_stage, item);
}

bool Quest::isSuitableForGivingQuest(int playerLevel) {
    return playerLevel >= requiredLevel;
}

bool Quest::isCompleted() const {
    return state == QuestState::COMPLETED;
}

bool Quest::isFinishObjectives() const {
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
    return dialogueIndex >= static_cast<int>(dialogues[stage].size());
}

bool Quest::accept() {
    if (state == QuestState::NOT_ACCEPTED) {
        state = QuestState::IN_PROGRESS;

        SoundManager::playSound("updateQuest");
        // std::cerr << "Quest accepted: '" << title << "' " << "stage: " << stage << "\n";

        return true;
    }

    return false;
}

bool Quest::updateStage() {
    if (state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives()) {
        ++stage;
        
        if (stage == static_cast<int>(npcIDs.size())) {
            state = QuestState::COMPLETED;
        }
        else {
            dialogueIndex = 0;
        }

        SoundManager::playSound("updateQuest");
        
        return true;
    }

    return false;
}

bool Quest::shouldGiveItemForPlayer() const {
    if (state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives()) {
        for (const auto& pair : itemFromNpc) {
            if (pair.first == stage) {
                return true;
            }
        }
    }

    return false;
}

void Quest::update(const QuestEventData& data) {
    if (state != QuestState::IN_PROGRESS) {
        return;
    }

    for (std::shared_ptr<QuestObjective>& objective : objectives[stage]) {
        objective->updateProgress(data);
    }
}

std::string Quest::getQuestInformation(const int& idx) const {
    std::string display = "[" + std::to_string(idx) + "] " + title + " ";

    switch (state) {
        case QuestState::NOT_ACCEPTED: {
            display += "[Not accepted]\n";
            display += "    " + requiredDescription + "\n";
            
            break;
        }
        case QuestState::IN_PROGRESS: {
            display += "[In progress]\n";
            if (descriptions[stage] != std::string()) {
                display += "    " + descriptions[stage] + "\n";
            }
            for (const std::shared_ptr<QuestObjective>& objective : objectives[stage]) {
                std::string str = objective->getDescription();
                if (str != std::string()) {
                    display += "- " + str + (objective->isFinished() ? " (Done)" : "") + "\n";
                }
            }

            break;
        }
        case QuestState::COMPLETED: {
            display += "[Completed]\n";

            break;
        }
    }

    return display;
}

std::vector<std::shared_ptr<ItemData>> Quest::getNpcItem() const {
    std::vector<std::shared_ptr<ItemData>> npcItems;
    if (state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives()) {
        for (const auto& pair : itemFromNpc) {
            if (pair.first == stage) {
                npcItems.push_back(pair.second);
            }
        }
    }
    return npcItems;
}

const std::vector<std::shared_ptr<QuestObjective>>& Quest::getQuestObjectives() const {
    return objectives[stage];
}

int Quest::getRewardExp() {
    if (state != QuestState::COMPLETED) {
        return 0;
    }

    rewardGiven = true;

    SoundManager::playSound("finishedQuest");

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
        return "[Bug] - Quest.cpp - getDialogue()";
    }
}

std::string Quest::getRequired() const {
    return "You must be at least Lv." + std::to_string(requiredLevel) + "\nto accept this quest";
}

void Quest::isInterruptedGivingQuest() {
    if (!isFinishedDialogue()) {
        dialogueIndex = 0;
    }
}