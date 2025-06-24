#pragma once

#include "ItemData.hpp"

#include <unordered_map>

class ItemDatabase {
private:
    static std::unordered_map<std::string, std::unique_ptr<ItemData>> itemStorage;

public:
    static std::shared_ptr<ItemData> get(const std::string& name, const int& amount = 1);

    static void loadItems();

};