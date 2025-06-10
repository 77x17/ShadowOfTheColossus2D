#include "TileMap.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

bool TileMap::load(const std::string& tmxPath, const std::string& tilesetPath)
{
    // Xóa dữ liệu cũ nếu có
    m_layersVertices.clear();

    if (!m_tilesetTexture.loadFromFile(tilesetPath))
    {
        std::cerr << "Failed to load tileset texture: " << tilesetPath << '\n';
        return false;
    }

    pugi::xml_document doc;
    if (!doc.load_file(tmxPath.c_str())) {
        std::cerr << "Failed to load TMX file: " << tmxPath << '\n';
        return false;
    }

    pugi::xml_node mapNode = doc.child("map");
    if (!mapNode) return false;

    // Kiểm tra xem map có phải là infinite không. Code này chỉ hỗ trợ map cố định.
    if (mapNode.attribute("infinite").as_bool()) {
        std::cerr << "Error: This loader only supports fixed-size maps. Please disable 'Infinite' in Tiled map properties." << '\n';
        return false;
    }

    unsigned int mapWidth = mapNode.attribute("width").as_uint();
    unsigned int mapHeight = mapNode.attribute("height").as_uint();
    unsigned int tileWidth = mapNode.attribute("tilewidth").as_uint();
    unsigned int tileHeight = mapNode.attribute("tileheight").as_uint();
    int firstGid = mapNode.child("tileset").attribute("firstgid").as_int();

    // Lặp qua tất cả các layer
    for (pugi::xml_node layerNode : mapNode.children("layer")) {
        pugi::xml_node dataNode = layerNode.child("data");
        if (!dataNode || std::string(dataNode.attribute("encoding").as_string()) != "csv") {
            continue; // Bỏ qua nếu không có data hoặc không phải CSV
        }

        // Với map cố định, ta đọc thẳng dữ liệu từ thẻ <data>
        std::string csvData = dataNode.text().get();
        std::vector<int> layerTileIds;
        std::stringstream ss(csvData);
        std::string tileIdStr;

        while (std::getline(ss, tileIdStr, ',')) {
            tileIdStr.erase(std::remove_if(tileIdStr.begin(), tileIdStr.end(), ::isspace), tileIdStr.end());
            if (!tileIdStr.empty()) {
                layerTileIds.push_back(std::stoi(tileIdStr));
            }
        }
        
        // Kiểm tra lại kích thước cho chắc chắn
        if (layerTileIds.size() != mapWidth * mapHeight) {
            std::cerr << "Layer '" << layerNode.attribute("name").as_string() 
                      << "' data size does not match map dimensions!" << '\n';
            continue; // Bỏ qua layer bị lỗi
        }

        // Tạo VertexArray cho layer này (phần này giữ nguyên)
        sf::VertexArray vertices;
        vertices.setPrimitiveType(sf::Quads);
        vertices.resize(mapWidth * mapHeight * 4);

        for (unsigned int i = 0; i < mapWidth; ++i) {
            for (unsigned int j = 0; j < mapHeight; ++j) {
                int tileGid = layerTileIds[i + j * mapWidth];
                if (tileGid == 0) continue;

                int localTileId = tileGid - firstGid;
                int tu = localTileId % (m_tilesetTexture.getSize().x / tileWidth);
                int tv = localTileId / (m_tilesetTexture.getSize().x / tileWidth);

                sf::Vertex* quad = &vertices[(i + j * mapWidth) * 4];

                quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
                quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
                quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
                quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
            }
        }
        m_layersVertices.push_back(vertices);
    }

    // Lặp qua tất cả các Object Group
    for (pugi::xml_node objectGroupNode : mapNode.children("objectgroup")) {
        std::string groupName = objectGroupNode.attribute("name").as_string();
        if (groupName == "CollisionObjects") {
            // Lặp qua tất cả các đối tượng trong group này
            for (pugi::xml_node objectNode : objectGroupNode.children("object")) {
                // Lấy vị trí và kích thước của đối tượng hình chữ nhật
                float x = objectNode.attribute("x").as_float();
                float y = objectNode.attribute("y").as_float();
                float width = objectNode.attribute("width").as_float();
                float height = objectNode.attribute("height").as_float();

                m_collisionRects.emplace_back(x, y, width, height);
            }
        }
    }

    return true;
}

// hàm draw() không thay đổi
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &m_tilesetTexture;

    for (const auto& layer : m_layersVertices) {
        target.draw(layer, states);
    }

    sf::RectangleShape hitbox;
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    for (const sf::FloatRect& rect : m_collisionRects) {
        hitbox.setSize(rect.getSize());
        hitbox.setPosition(rect.getPosition());
        target.draw(hitbox);
    }
}

void TileMap::updateCollisionRects() {
    for (sf::FloatRect& rect : m_collisionRects) {
        rect = getTransform().transformRect(rect);
    }
}

const std::vector<sf::FloatRect>& TileMap::getCollisionRects() const {
    return m_collisionRects;
}