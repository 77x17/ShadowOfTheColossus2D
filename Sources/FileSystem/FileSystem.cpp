#include "FileSystem.hpp"

#include "json.hpp"
#include <fstream>

#include "Player.hpp"

void FileSystem::saveGame(const Player& player) {
    std::filesystem::path saveDir = "./Saves";
    if (!std::filesystem::exists(saveDir)) { std::filesystem::create_directories(saveDir); }

    nlohmann::json saveData;

    saveData["player"]["position"]["x"] = player.getPosition().x;
    saveData["player"]["position"]["y"] = player.getPosition().y;
    saveData["player"]["level"]         = player.getLevel();
    saveData["player"]["xp"]            = player.getXp();
    saveData["player"]["inventory"]     = player.getInventoryString(); 
    saveData["player"]["equipment"]     = player.getEquipmentString();
    std::vector<QuestProgressData> quests = player.getQuestsData();
    for (const QuestProgressData& quest : quests) {
        nlohmann::json questJson;
        questJson["questID"]       = quest.questID;
        questJson["stage"]         = quest.stage;
        questJson["dialogueIndex"] = quest.dialogueIndex;
        questJson["state"]         = static_cast<int>(quest.state);
        questJson["rewardGiven"]   = quest.rewardGiven;

        saveData["player"]["quests"].push_back(questJson);
    }

    std::ofstream file(saveDir / "save.json");
    if (file.is_open()) {
        file << saveData.dump(4);
    }
    else {
        std::cerr << "[Bug] - FileSystem.cpp - saveGame()\n";
    }
}

std::vector<QuestProgressData> extractQuestsFromJson(const nlohmann::json& json) {
    std::vector<QuestProgressData> quests;

    if (!json.is_array()) return quests;

    for (const auto& item : json) {
        QuestProgressData quest(0, 0, 0, QuestState::LOCK, false);
        quest.questID       = item.value("questID", 0);
        quest.stage         = item.value("stage", 0);
        quest.dialogueIndex = item.value("dialogueIndex", 0);
        quest.state         = static_cast<QuestState>(item.value("state", 0)); // chú ý nếu state là enum
        quest.rewardGiven   = item.value("rewardGiven", false);

        quests.push_back(quest);
    }

    return quests;
}

void FileSystem::loadSaveGame(Player& player) {
    std::filesystem::path savePath = "./Saves/save.json";

    std::ifstream file(savePath);
    if (!file.is_open()) {
        std::cerr << "[Bug] - FileSystem.cpp - loadSaveGame()\n";
        return;
    }

    nlohmann::json saveData;
    try {
        file >> saveData;

        player.setPosition(sf::Vector2f(saveData["player"]["position"]["x"], saveData["player"]["position"]["y"]));
        player.setLevel(saveData["player"]["level"]);
        player.setXp(saveData["player"]["xp"]);
        player.setInventory(saveData["player"]["inventory"]);
        player.setEquipment(saveData["player"]["equipment"]);
        player.setQuests(extractQuestsFromJson(saveData["player"]["quests"]));
        player.modifierAfterLoad();
    }
    catch (const std::exception& e) { 
        std::cerr << "[Bug] - FileSystem.cpp - loadSaveGame(): " << e.what() << "\n";
    }
}