#pragma once

#include <string>

class QuestObjective {
public:
    virtual ~QuestObjective() = default;
    virtual void updateProgress(const std::string& eventType, const std::string& target) = 0;
    virtual bool isFinished() const = 0;
    virtual std::string getDescription() const = 0;
};