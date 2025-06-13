#include "ShaderManager.hpp"

void ShaderManager::load(const std::string& id, const std::string& path, const std::unordered_map<std::string, float>& uniforms) {
    auto shader = std::make_unique<sf::Shader>();
    if (shader->loadFromFile(path, sf::Shader::Fragment)) {
        for (const auto& [name, value] : uniforms) {
            shader->setUniform(name, value);
        }
        shaders[id] = std::move(shader);
    } else {
        std::cerr << "Failed to load shader: " << path << '\n';
    }
}

sf::Shader* ShaderManager::get(const std::string& id) {
    auto it = shaders.find(id);
    if (it != shaders.end()) {
        return it->second.get();
    }
    return nullptr;
}