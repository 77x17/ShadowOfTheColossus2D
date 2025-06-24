#pragma once

#include "Quest.hpp"

#include "KillMonsterObjective.hpp"
#include "FinishObjective.hpp"
#include "TalkObjective.hpp"
#include "ExploreObjective.hpp"
#include "GiveItemObjective.hpp"
#include "CollectItemObjective.hpp"

class QuestManager {
private:
    std::vector<Quest> quests;
public:
    void loadQuests();

    std::vector<Quest>& getQuests();
};