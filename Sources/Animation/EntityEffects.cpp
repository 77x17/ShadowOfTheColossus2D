#include "EntityEffects.hpp"

void EntityEffects::load(const std::string& id, const std::string& path, const std::unordered_map<std::string, UniformVariant>& uniforms) {
    std::unique_ptr<sf::Shader> shader = std::make_unique<sf::Shader>();
    if (shader->loadFromFile(path, sf::Shader::Fragment)) {
        for (const auto& [name, value] : uniforms) {
            // shader->setUniform(name, value);
            std::visit([&](auto&& v) {
                shader->setUniform(name, v);
            }, value);
        }
        shaders[id] = std::move(shader);
    } else {
        std::cerr << "Failed to load shader: " << path << '\n';
    }
}

sf::Shader* EntityEffects::get(const std::string& id) {
    auto it = shaders.find(id);
    if (it != shaders.end()) {
        return it->second.get();
    } else {
        std::cerr << "[Bug] - EntityEffects.cpp get()\n";
    }

    return nullptr;
}
