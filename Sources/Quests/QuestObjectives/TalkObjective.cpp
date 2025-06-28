#include "TalkObjective.hpp"

TalkObjective::TalkObjective(int _npcID) {
    npcID = _npcID;
    met   = false;
}

void TalkObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "talk" && data.npcID == npcID) {
        met = true;
    }
}

bool TalkObjective::isFinished() const {
    return met;
}

std::string TalkObjective::getDescription() const {
    return std::string();
}

std::shared_ptr<QuestObjective> TalkObjective::clone() const {
    return std::make_shared<TalkObjective>(*this);
}