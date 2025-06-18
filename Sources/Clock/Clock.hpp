#pragma once

#include <string>

class Clock {
private:
    float minutes;

public:
    Clock(const float& _minutes = 360.0f);

    void update(const float& dt);
    int getHour() const;
    int getMinute() const;

    std::string getTimeString() const;
    
    std::string getTimeOfDay() const;

};