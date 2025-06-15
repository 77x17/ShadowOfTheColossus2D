#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>

#include "pugixml.hpp"

// Tilemap Animation
struct TileMapAnimationFrame {
    int tileID;
    float duration;

    TileMapAnimationFrame(int id, float dur) : tileID(id), duration(dur) {}
};

struct TileMapAnimation {
    std::vector<TileMapAnimationFrame> frames;
};

class TileMap : public sf::Drawable, public sf::Transformable {
private:
    unsigned int mapWidth;
    unsigned int mapHeight;
    unsigned int tileWidth;
    unsigned int tileHeight;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    std::vector<sf::VertexArray> m_layersVertices; 
    std::unordered_map<std::string, sf::Texture> m_tilesetTextures;
    // std::unordered_map<int, std::string> m_gidToTilesetName;        // GID → tên tileset
    struct TilesetInfo {
        std::string name;
        int firstGid;
    };
    std::map<int, std::string> m_gidStartToTileset;
    TilesetInfo getTilesetInfoForGid(int gid) const;

    std::vector<std::pair<std::string, sf::VertexArray>> m_layerData; // texture name + layer

    std::vector<sf::FloatRect> m_collisionRects;
    std::unordered_map<int, sf::FloatRect> m_NPCRects;
    std::unordered_map<std::string, std::vector<sf::FloatRect>> m_enemyRects;
    std::unordered_map<int, sf::FloatRect> m_RegionRects;

    // Tilemap Animation
    struct AnimatedTile {
        const TileMapAnimation* animationData;
        sf::VertexArray vertices;
        int currentFrame;
        float elapsedTime = 0.0f;

        std::string tilesetName;
        std::size_t vertexIndex;
    };
    std::unordered_map<int, TileMapAnimation> m_animations;

    std::vector<AnimatedTile> m_animatedTiles;

public:
    bool load(const std::string& tmxPath, const std::vector<std::pair<std::string, std::string>>& tilesets);

    void updateObjects();

    sf::FloatRect getGlobalBounds() const;
    sf::Vector2u getPixelSize() const;
    void drawMinimap(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    const std::vector<sf::FloatRect>& getCollisionRects() const;
    const std::unordered_map<int, sf::FloatRect>& getNpcRects() const;
    const std::unordered_map<std::string, std::vector<sf::FloatRect>>& getEnemyRects() const;
    const std::unordered_map<int, sf::FloatRect>& getRegionRects() const;

    // Tilemap Animation
    void update(const float& dt);

};