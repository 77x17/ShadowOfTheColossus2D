#pragma once

#include "QuestObjective.hpp"

class TalkObjective : public QuestObjective {
    int  npcID;
    bool met;
public:
    TalkObjective(int _npcID);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;
};