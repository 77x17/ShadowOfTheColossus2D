#include "TalkingObjective.hpp"

TalkingObjective::TalkingObjective(int _npcID) {
    npcID = _npcID;
    met   = false;
}

void TalkingObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "talk" && data.targetNpcID == npcID) {
        met = true;
    }
}

bool TalkingObjective::isFinished() const {
    return met;
}

std::string TalkingObjective::getDescription() const {
    return "";
}