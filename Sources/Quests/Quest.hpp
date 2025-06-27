#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "ItemData.hpp"

enum class QuestState {
    LOCK,
    NOT_ACCEPTED,
    IN_PROGRESS,
    COMPLETED
};

class Quest {
private:
    int questID;

    std::string title;
    std::string requiredDescription;
    std::vector<int> npcIDs;
    std::vector<std::vector<std::string>> dialogues;
    std::vector<std::string> descriptions;
    std::vector<std::vector<std::shared_ptr<QuestObjective>>> objectives;
    std::vector<std::pair<int, std::shared_ptr<ItemData>>> itemFromNpc;
    int requiredLevel;
    int requiredQuestID;

    int rewardExp;

public:
    Quest(int m_questID, const std::string& m_title, int exp);
    
    void addRequiredLevel(int level);
    void addRequiredQuestID(int m_questID);
    void addRequiredDescription(const std::string& description);
    void addNpcID      (int m_stage, int npcID);
    void addDialogue   (int m_stage, const std::string& dialogue);
    void addDescription(int m_stage, const std::string& description);
    void addObjective  (int m_stage, const std::shared_ptr<QuestObjective>& objective);
    void addItemFromNpc(int m_stage, const std::shared_ptr<ItemData>& item);

    bool isFinishedDialogue(int m_stage, int m_dialogueIndex) const;
    bool shouldGiveItemForPlayer(int m_stage) const;

    bool updateStage(int& m_stage, QuestState& m_state) const;

    int getNpcID(int m_stage) const;
    int getRequiredQuestID() const;
    int getRequiredLevel() const;
    int getRewardExp() const;
    std::string getDialogue(int m_stage, int m_dialogueIndex) const;
    std::string getRequiredLevelString() const;
    std::string getQuestInformation(QuestState state, int m_stage, int index) const;
    const std::vector<std::shared_ptr<ItemData>> getNpcItem(int m_stage) const;
    const std::vector<std::shared_ptr<QuestObjective>>& getQuestObjectives(const int& m_stage) const;

};