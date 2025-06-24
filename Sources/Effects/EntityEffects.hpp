#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <variant>

using UniformVariant = std::variant<float, sf::Glsl::Vec4, bool>;

class EntityEffects {
private:
    static std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaders;

public:
    static void load(const std::string& id, const std::string& path, const std::unordered_map<std::string, UniformVariant>& uniforms);

    static sf::Shader* get(const std::string& id);

    static void loadShaders();
};