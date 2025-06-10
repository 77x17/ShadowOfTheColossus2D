#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#include "pugixml.hpp"

class TileMap : public sf::Drawable, public sf::Transformable {
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<sf::VertexArray> m_layersVertices; 
    sf::Texture m_tilesetTexture;

    std::vector<sf::FloatRect> m_collisionRects;

public:
    bool load(const std::string& tmxPath, const std::string& tilesetPath);

    void updateCollisionRects();

    const std::vector<sf::FloatRect>& getCollisionRects() const;

};