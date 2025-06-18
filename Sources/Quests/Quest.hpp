#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

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
    int requiredLevel;

    QuestState state;
    bool rewardGiven;
    int  rewardExp;

    int  stage;
    int  dialogueIndex;
    bool updateStage;

public:
    Quest(const std::string& _title, int exp);
    
    void addRequiredLevel(int level);
    void addNpcID      (int _stage, int ID);
    void addDialogue   (int _stage, const std::string& dialogue);
    void addDescription(int _stage, const std::string& description);
    void addObjective  (int _stage, const std::shared_ptr<QuestObjective>& objective);

    bool isSuitableForGivingQuest(int playerLevel);
    bool isCompleted() const;
    bool isFinishObjectives() const;
    bool isReceiveReward() const;
    bool isFinishedDialogue() const;
    bool isUpdateStage();

    bool accept();
    void nextStage();
    void update(const QuestEventData& data);

    std::string getQuestInformation(const int& idx) const;

    int getRewardExp();

    int getID() const;
    std::string getDialogue();
    std::string getRequired() const;

    void isInterruptedGivingQuest();
};