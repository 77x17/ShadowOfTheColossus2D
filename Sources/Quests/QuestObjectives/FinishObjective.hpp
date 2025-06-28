#pragma once

#include "QuestObjective.hpp"

class FinishObjective : public QuestObjective {
public:
    void updateProgress(const QuestEventData& data) override;

    bool isFinished() const override;

    std::string getDescription() const override;

    std::shared_ptr<QuestObjective> clone() const override;
};