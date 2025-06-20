#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "ItemData.hpp"

enum class QuestState {
    NOT_ACCEPTED,
    IN_PROGRESS,
    COMPLETED
};

class Quest {
private:
    std::string      title;
    std::vector<int> npcIDs;
    std::vector<std::vector<std::string>> dialogues;
    std::vector<std::string> descriptions;
    std::vector<std::vector<std::shared_ptr<QuestObjective>>> objectives;
    std::vector<std::pair<int, std::shared_ptr<ItemData>>> itemFromNpc;
    int requiredLevel;

    QuestState state;
    bool rewardGiven;
    int  rewardExp;

    int  stage;
    int  dialogueIndex;

    std::string requiredDescription;

public:
    Quest(const std::string& _title, int exp);
    
    void addRequiredLevel(int level);
    void addRequiredDescription(const std::string& description);
    void addNpcID      (int _stage, int ID);
    void addDialogue   (int _stage, const std::string& dialogue);
    void addDescription(int _stage, const std::string& description);
    void addObjective  (int _stage, const std::shared_ptr<QuestObjective>& objective);
    void addItemFromNpc(int _stage, const std::shared_ptr<ItemData>& item);

    bool isSuitableForGivingQuest(int playerLevel);
    bool isCompleted() const;
    bool isFinishObjectives() const;
    bool isReceiveReward() const;
    bool isFinishedDialogue() const;

    bool accept();
    bool updateStage();
    bool shouldGiveItemForPlayer() const;
    void update(const QuestEventData& data);

    std::string getQuestInformation(const int& idx) const;
    std::vector<std::shared_ptr<ItemData>> getNpcItem() const;
    const std::vector<std::shared_ptr<QuestObjective>>& getQuestObjectives() const;

    int getRewardExp();

    int getID() const;
    std::string getDialogue();
    std::string getRequired() const;

    void isInterruptedGivingQuest();

};