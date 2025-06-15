#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <iostream>
#include <memory>

class EntityEffects {
private:
    static std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaders;

public:
    static void load(const std::string& id, const std::string& path, const std::unordered_map<std::string, float>& uniforms);

    static sf::Shader* get(const std::string& id);
};