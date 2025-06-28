#include "QuestProgress.hpp"

#include "QuestDatabase.hpp"
#include "SoundManager.hpp"

QuestProgress::QuestProgress(int m_questID)
: data(m_questID, 0, 0, QuestState::LOCK, false) {
    quest      = &QuestDatabase::getQuestByID(data.questID);
    objectives = quest->getQuestObjectives(data.stage);
}

bool QuestProgress::isFinishedDialogue() const {
    return quest->isFinishedDialogue(data.stage, data.dialogueIndex);
}

bool QuestProgress::isFinishObjectives() const {
    for (const std::shared_ptr<QuestObjective>& objective : objectives) {
        if (!objective->isFinished()) {
            return false;
        }
    }

    return true;
}

void QuestProgress::isInterruptedGivingQuest() {
    if (!isFinishedDialogue()) {
        data.dialogueIndex = 0;
    }
}

void QuestProgress::update(const QuestEventData& dataPack) {
    if (data.state != QuestState::IN_PROGRESS) {
        return;
    }

    for (std::shared_ptr<QuestObjective>& objective : objectives) {
        objective->updateProgress(dataPack);
    }
}

bool QuestProgress::isCompleted() const {
    return data.state == QuestState::COMPLETED;
}

bool QuestProgress::isLocked() const {
    return data.state == QuestState::LOCK;
}

void QuestProgress::unlock(int currentQuestID) {
    if (data.state == QuestState::LOCK && currentQuestID >= quest->getRequiredQuestID()) {
        data.state = QuestState::NOT_ACCEPTED;
    }
}

int QuestProgress::getQuestID() const {
    return data.questID;
}

bool QuestProgress::shouldGiveItemForPlayer() const {
    if (data.state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives() && quest->shouldGiveItemForPlayer(data.stage)) {
        return true;
    }

    return false;
}

std::vector<std::shared_ptr<ItemData>> QuestProgress::getNpcItem() const {
    return quest->getNpcItem(data.stage);
}

bool QuestProgress::updateStage() {
    if (data.state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives()) {
        if (quest->updateStage(data.stage, data.state)) {
            data.dialogueIndex = 0;
            
            if (!isCompleted()) {
                objectives = quest->getQuestObjectives(data.stage);
             
                SoundManager::playSound("updateQuest");
            }
        }
        
        return true;
    }

    return false;
}

bool QuestProgress::isRewardReceived() const {
    return data.rewardGiven == true;
}

int QuestProgress::getRewardExp() {
    data.rewardGiven = true;

    SoundManager::playSound("finishedQuest");

    return quest->getRewardExp();
}

const std::vector<std::shared_ptr<QuestObjective>>& QuestProgress::getQuestObjectives() const {
    return objectives;
}

int QuestProgress::getNpcID() const {
    return quest->getNpcID(data.stage);
}

bool QuestProgress::isSuitableForGivingQuest(int playerLevel) const {
    return playerLevel >= quest->getRequiredLevel();
}

bool QuestProgress::accept() {
    if (data.state == QuestState::NOT_ACCEPTED) {
        data.state = QuestState::IN_PROGRESS;

        SoundManager::playSound("updateQuest");

        return true;
    }

    return false;
}

std::string QuestProgress::getDialogue() {
    return quest->getDialogue(data.stage, data.dialogueIndex++);
}

std::string QuestProgress::getRequiredLevelString() const {
    return quest->getRequiredLevelString();
}

std::string QuestProgress::getQuestInformation(int index) const {
    std::string display = quest->getQuestInformation(data.state, data.stage, index);;
    if (data.state == QuestState::IN_PROGRESS) {
        for (const std::shared_ptr<QuestObjective>& objective : objectives) {
            std::string str = objective->getDescription();
            if (str != std::string()) {
                display += "- " + str + (objective->isFinished() ? " (Done)" : "") + "\n";
            }
        }
    }
    return display;
}

QuestProgressData QuestProgress::getData() const {
    return data;
}

void QuestProgress::setData(const QuestProgressData& m_data) {
    data = m_data;
    if (data.state != QuestState::COMPLETED) {
        objectives = quest->getQuestObjectives(data.stage);
    }
    else {
        objectives = std::vector<std::shared_ptr<QuestObjective>>();
    }
}