#pragma once

#include "QuestObjective.hpp"

#include <iostream>
#include <vector>
#include <memory>

enum class QuestState {
    NOT_ACCEPTED,
    IN_PROGRESS,
    READY_TO_TURN_IN,
    COMPLETED
};

class Quest {
private:
    std::string      title;
    std::vector<int> npcIDs;
    std::vector<std::vector<std::string>> dialogues;
    std::vector<std::string> descriptions;
    std::vector<std::vector<std::shared_ptr<QuestObjective>>> objectives;
    std::vector<bool> needToTurnIn;
    int requiredLevel;

    QuestState state;
    bool rewardGiven;
    int  rewardExp;

    int  stage;
    int  dialogueIndex;
    bool turnInNotification;

public:
    Quest(const std::string& _title, int exp);
    
    void addRequiredLevel(int level);
    void addNpcID      (int _stage, int ID);
    void addDialogue   (int _stage, const std::string& dialogue, bool create = false);
    void addDescription(int _stage, const std::string& description);
    void addObjective  (int _stage, const std::shared_ptr<QuestObjective>& objective, bool create = false);
    void setTurnIn     (int _stage, bool turnIn);

    bool isSuitableForGivingQuest(int playerLevel);
    bool isCompleted() const;
    bool isFinishedObjectives() const;
    bool isReceiveReward() const;
    bool isFinishedDialogue() const;
    bool isReadyToTurnIn();

    bool accept();
    void nextStage();
    void update(const QuestEventData& data);
    bool turnIn();

    std::string getQuestInformation(const int& idx) const;

    int getRewardExp();

    int getID() const;
    std::string getDialogue();
    std::string getRequired() const;

    void isInterruptedGivingQuest();
};