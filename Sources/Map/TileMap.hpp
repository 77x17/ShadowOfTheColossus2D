#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "pugixml.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<sf::VertexArray> m_layersVertices; 
    sf::Texture m_tilesetTexture;

    std::vector<sf::FloatRect> m_collisionRects;
    std::vector<std::pair<int, sf::FloatRect>> m_NPCRects;
    std::unordered_map<std::string, std::vector<sf::FloatRect>> m_enemyRects;

public:
    bool load(const std::string& tmxPath, const std::string& tilesetPath);

    void updateObjects();

    const std::vector<sf::FloatRect>& getCollisionRects() const;
    const std::vector<std::pair<int, sf::FloatRect>>& getNPCRects() const;
    const std::unordered_map<std::string, std::vector<sf::FloatRect>>& getEnemyRects() const;

};