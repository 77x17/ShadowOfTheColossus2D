#pragma once

#include <string>

struct QuestEventData {
    std::string eventType;              
    std::string targetName;             
    int         npcID    = -1;     
    int         regionID = -1;     
    int         amount   = 0;     
};

class QuestObjective {
public:
    virtual ~QuestObjective() = default;
    virtual void updateProgress(const QuestEventData& data) = 0;
    virtual bool isFinished() const = 0;
    virtual std::string getDescription() const = 0;
    virtual QuestEventData getQuestEventData() const {
        return QuestEventData{};
    }

};