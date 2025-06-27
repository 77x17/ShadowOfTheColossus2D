#include "Quest.hpp"

#include "SoundManager.hpp"

Quest::Quest(int m_questID, const std::string& m_title, int exp) {
    questID = m_questID;
    title = m_title;
    npcIDs.clear();
    dialogues.clear();
    descriptions.clear();
    objectives.clear();

    requiredLevel = 0;
    requiredQuestID = -1;

    rewardExp   = exp;
}

void Quest::addRequiredLevel(int level) {
    requiredLevel = level;
}

void Quest::addRequiredQuestID(int m_questID) {
    requiredQuestID = m_questID;
}

void Quest::addRequiredDescription(const std::string& description) {
    requiredDescription = description;
}

void Quest::addNpcID(int m_stage, int npcID) {
    npcIDs.push_back(npcID);
}

void Quest::addDialogue(int m_stage, const std::string& dialogue) {
    if (m_stage >= static_cast<int>(dialogues.size())) {
        dialogues.push_back(std::vector<std::string>());
    }
    
    if (dialogue != std::string()) {
        dialogues[m_stage].push_back(dialogue);
    }
}

void Quest::addDescription(int m_stage, const std::string& description) {
    descriptions.push_back(description);
}

void Quest::addObjective(int m_stage, const std::shared_ptr<QuestObjective>& objective) {
    if (m_stage >= static_cast<int>(objectives.size())) {
        objectives.push_back(std::vector<std::shared_ptr<QuestObjective>>());
    }

    objectives[m_stage].push_back(objective);
}

void Quest::addItemFromNpc(int m_stage, const std::shared_ptr<ItemData>& item) {
    itemFromNpc.emplace_back(m_stage, item);
}

bool Quest::isFinishedDialogue(int m_stage, int m_dialogueIndex) const {
    return m_dialogueIndex >= static_cast<int>(dialogues[m_stage].size());
}

bool Quest::updateStage(int& m_stage, QuestState& m_state) const {
    ++m_stage;
    
    if (m_stage == static_cast<int>(npcIDs.size())) {
        m_state = QuestState::COMPLETED;
    }
    else {
        return true;
    }

    return false;
}

bool Quest::shouldGiveItemForPlayer(int m_stage) const {
    for (const auto& pair : itemFromNpc) {
        if (pair.first == m_stage) {
            return true;
        }
    }

    return false;
}

std::string Quest::getQuestInformation(QuestState state, int m_stage, int index) const {
    std::string display = "[" + std::to_string(index) + "] " + title + " ";

    switch (state) {
        case QuestState::LOCK: {
            display += "[Locked]\n";

            break;
        }
        case QuestState::NOT_ACCEPTED: {
            display += "[Not accepted]\n";
            display += "    " + requiredDescription + "\n";
            
            break;
        }
        case QuestState::IN_PROGRESS: {
            display += "[In progress]\n";
            if (descriptions[m_stage] != std::string()) {
                display += "    " + descriptions[m_stage] + "\n";
            }
            for (const std::shared_ptr<QuestObjective>& objective : objectives[m_stage]) {
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

const std::vector<std::shared_ptr<QuestObjective>>& Quest::getQuestObjectives(const int& m_stage) const {
    if (m_stage >= static_cast<int>(objectives.size())) {
        std::cerr << "[Bug] - Quest.cpp - getQuestObjectives()\n";
    }

    return objectives[m_stage];
}

int Quest::getRewardExp() const {
    return rewardExp;
}

int Quest::getNpcID(int m_stage) const {
    if (m_stage >= static_cast<int>(npcIDs.size())) {
        std::cerr << "[Bug] - Quest.cpp - getNpcID()\n";
    }

    return npcIDs[m_stage];
}

std::string Quest::getDialogue(int m_stage, int m_dialogueIndex) const {
    if (m_dialogueIndex >= static_cast<int>(dialogues[m_stage].size())) {
        std::cerr << "[Bug] - Quest.cpp - getDialogue()\n";
    }
    
    return dialogues[m_stage][m_dialogueIndex];
}

std::string Quest::getRequiredLevelString() const {
    return "You must be at least Lv." + std::to_string(requiredLevel) + "\nto accept this quest";
}

int Quest::getRequiredQuestID() const {
    return requiredQuestID;
}

const std::vector<std::shared_ptr<ItemData>> Quest::getNpcItem(int m_stage) const {
    std::vector<std::shared_ptr<ItemData>> bags;

    for (const auto& pair : itemFromNpc) {
        if (pair.first == m_stage) {
            bags.push_back(pair.second->clone());
        }
    }

    return bags;
}

int Quest::getRequiredLevel() const {
    return requiredLevel;
}