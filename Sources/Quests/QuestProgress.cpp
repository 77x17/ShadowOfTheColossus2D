#include "QuestProgress.hpp"

#include "QuestDatabase.hpp"
#include "SoundManager.hpp"

QuestProgress::QuestProgress(int questID)
: questID(questID), stage(0), dialogueIndex(0), state(QuestState::LOCK), rewardGiven(false) {
    quest      = &QuestDatabase::getQuestByID(questID);
    objectives = quest->getQuestObjectives(stage);
}

bool QuestProgress::isFinishedDialogue() const {
    return quest->isFinishedDialogue(stage, dialogueIndex);
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
        dialogueIndex = 0;
    }
}

void QuestProgress::update(const QuestEventData& dataPack) {
    if (state != QuestState::IN_PROGRESS) {
        return;
    }

    for (std::shared_ptr<QuestObjective>& objective : objectives) {
        objective->updateProgress(dataPack);
    }
}

bool QuestProgress::isCompleted() const {
    return state == QuestState::COMPLETED;
}

bool QuestProgress::isLocked() const {
    return state == QuestState::LOCK;
}

void QuestProgress::unlock(int currentQuestID) {
    if (state == QuestState::LOCK && currentQuestID >= quest->getRequiredQuestID()) {
        state = QuestState::NOT_ACCEPTED;
    }
}

int QuestProgress::getQuestID() const {
    return questID;
}

bool QuestProgress::shouldGiveItemForPlayer() const {
    if (state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives() && quest->shouldGiveItemForPlayer(stage)) {
        return true;
    }

    return false;
}

std::vector<std::shared_ptr<ItemData>> QuestProgress::getNpcItem() const {
    return quest->getNpcItem(stage);
}

bool QuestProgress::updateStage() {
    if (state == QuestState::IN_PROGRESS && isFinishedDialogue() && isFinishObjectives()) {
        if (quest->updateStage(stage, state)) {
            dialogueIndex = 0;
            
            if (!isCompleted()) {
                objectives = quest->getQuestObjectives(stage);
             
                SoundManager::playSound("updateQuest");
            }
        }
        
        return true;
    }

    return false;
}

bool QuestProgress::isRewardReceived() const {
    return rewardGiven == true;
}

int QuestProgress::getRewardExp() {
    rewardGiven = true;

    SoundManager::playSound("finishedQuest");

    return quest->getRewardExp();
}

const std::vector<std::shared_ptr<QuestObjective>>& QuestProgress::getQuestObjectives() const {
    return objectives;
}

int QuestProgress::getNpcID() const {
    return quest->getNpcID(stage);
}

bool QuestProgress::isSuitableForGivingQuest(int playerLevel) const {
    return playerLevel >= quest->getRequiredLevel();
}

bool QuestProgress::accept() {
    if (state == QuestState::NOT_ACCEPTED) {
        state = QuestState::IN_PROGRESS;

        SoundManager::playSound("updateQuest");

        return true;
    }

    return false;
}

std::string QuestProgress::getDialogue() {
    return quest->getDialogue(stage, dialogueIndex++);
}

std::string QuestProgress::getRequiredLevelString() const {
    return quest->getRequiredLevelString();
}

std::string QuestProgress::getQuestInformation(int index) const {
    return quest->getQuestInformation(state, stage, index);
}