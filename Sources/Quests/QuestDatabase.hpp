#pragma once

#include <vector>

#include "Quest.hpp"

class QuestDatabase {
private:
    static std::vector<Quest> quests;

public:
    static void loadQuests();
    static const Quest& getQuestByID(const int& questID);
    static const std::vector<Quest>& getAllQuests();
    static const int getQuestsSize();

};