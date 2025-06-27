#pragma once

#include "Quest.hpp"

class QuestProgress {
private:
    int questID;
    int stage;
    int dialogueIndex;
    QuestState state;
    bool rewardGiven;

    std::vector<std::shared_ptr<QuestObjective>> objectives;
    const Quest* quest;

    void syncObjectives();

public:
    QuestProgress(int questID);

    bool isFinishedDialogue() const;
    bool isFinishObjectives() const;
    void isInterruptedGivingQuest();
    bool isCompleted() const;
    bool isLocked() const;
    bool isRewardReceived() const;
    bool isSuitableForGivingQuest(int playerLevel) const;
    bool shouldGiveItemForPlayer() const;

    void update(const QuestEventData& dataPack);
    void unlock(int currentQuestID);
    bool updateStage();
    bool accept();

    int getNpcID() const;
    int getRewardExp();
    int getQuestID() const;
    std::string getDialogue();
    std::string getRequiredLevelString() const;
    std::string getQuestInformation(int index) const;
    std::vector<std::shared_ptr<ItemData>> getNpcItem() const;
    const std::vector<std::shared_ptr<QuestObjective>>& getQuestObjectives() const;
};