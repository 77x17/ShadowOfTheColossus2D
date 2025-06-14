#pragma once

#include "QuestObjective.hpp"

class TalkingObjective : public QuestObjective {
    int npcID;
    bool met;
public:
    TalkingObjective(int _npcID);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;
};