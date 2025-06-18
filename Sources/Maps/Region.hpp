#pragma once

#include <string>

class Region {
public:
    static std::string getName(int regionID) {
        switch (regionID) {
            case 0:
                return "Elderfall Village";
            case 1:
                return "Greenthorn Grove";
            case 2:
                return "G.Grove Upper";
            default:
                return "Unknown Area";
        }
        
        std::cerr << "[Bug] - Region.hpp\n";
    }
};