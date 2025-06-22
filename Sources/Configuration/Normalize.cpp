#include "Normalize.hpp"

#include "Constants.hpp"
#include <cmath>

sf::Vector2f Normalize::normalize(const sf::Vector2f& source) {
    float length = std::sqrt((source.x * source.x) + (source.y * source.y));
    if (length > ZERO_EPSILON) {
        return sf::Vector2f(source.x / length, source.y / length);
    }
    else {
        // std::cerr << "[Bug] - Projectile.cpp - normalize()\n";
    }
    // Return the original vector if its length is 0 to avoid division by zero.
    return source;
}
    
