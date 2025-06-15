#include "ExploreObjective.hpp"

ExploreObjective::ExploreObjective(int _regionID) {
    regionID = _regionID;
    explored   = false;
}

void ExploreObjective::updateProgress(const QuestEventData& data) {
    if (data.eventType == "explore" && data.regionID == regionID) {
        explored = true;
    }
}

bool ExploreObjective::isFinished() const {
    return explored;
}

std::string ExploreObjective::getDescription() const {
    return std::string();
}