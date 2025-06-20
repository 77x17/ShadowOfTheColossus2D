#include "Clock.hpp"

#include <iomanip>
#include <sstream>

Clock::Clock(const float& _minutes) {
    minutes = _minutes; // 6h
}

void Clock::update(const float& dt) {
    minutes += dt * 15.0f;
    if (minutes >= 24 * 60.0f) minutes -= 24 * 60.0f;
}

int Clock::getHour() const {
    return static_cast<int>(minutes) / 60;
}

int Clock::getMinute() const {
    return static_cast<int>(minutes) % 60;
}

std::string Clock::getTimeString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << getHour() << ":"
        << std::setw(2) << std::setfill('0') << getMinute();
    return oss.str();
}


std::string Clock::getTimeOfDay() const {
    int hour = getHour();
    if (hour >= 05 && hour < 11) return "Morning";
    if (hour >= 11 && hour < 14) return "Noon";
    if (hour >= 14 && hour < 18) return "Afternoon";
    if (hour >= 18 && hour < 21) return "Evening";
    return "Night";
}