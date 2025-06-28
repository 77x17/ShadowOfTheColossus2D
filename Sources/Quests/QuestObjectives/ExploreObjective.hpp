#pragma once

#include "QuestObjective.hpp"

class ExploreObjective : public QuestObjective {
    int  regionID;
    bool explored;
public:
    ExploreObjective(int _regionID);

    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;

    std::shared_ptr<QuestObjective> clone() const override;
};