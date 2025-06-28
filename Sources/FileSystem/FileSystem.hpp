#pragma once

class Player;

class FileSystem {
public:

    static void saveGame(const Player& player);

    static void loadSaveGame(Player& player);
};