#pragma once

#include "Quest.hpp" 

struct QuestProgressData {
    int questID;
    int stage;
    int dialogueIndex;
    QuestState state;
    bool rewardGiven;

    QuestProgressData(int m_questID, int m_stage, int m_dialogueIndex, QuestState m_state, bool m_rewardGiven) 
    : questID(m_questID), stage(m_stage), dialogueIndex(m_dialogueIndex), state(m_state), rewardGiven(m_rewardGiven) {}
};

class QuestProgress {
private:
    QuestProgressData data;

    std::vector<std::shared_ptr<QuestObjective>> objectives;
    const Quest* quest;

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
    QuestProgressData getData() const;
    std::vector<std::shared_ptr<ItemData>> getNpcItem() const;
    const std::vector<std::shared_ptr<QuestObjective>>& getQuestObjectives() const;

    void setData(const QuestProgressData& m_data);
};