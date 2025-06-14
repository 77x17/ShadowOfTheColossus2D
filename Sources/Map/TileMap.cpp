#include "TileMap.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

bool TileMap::load(const std::string& tmxPath, const std::vector<std::pair<std::string, std::string>>& tilesets) {
    // Xóa dữ liệu cũ nếu có
    m_layersVertices.clear();
    m_tilesetTextures.clear();

    for (const auto& [tilesetName, tilesetPath] : tilesets) {
        sf::Texture texture;
        if (!texture.loadFromFile(tilesetPath)) {
            std::cerr << "Failed to load tileset: " << tilesetPath << '\n';
            return false;
        }
        m_tilesetTextures[tilesetName] = std::move(texture);
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

    mapWidth     = mapNode.attribute("width").as_uint();
    mapHeight    = mapNode.attribute("height").as_uint();
    tileWidth    = mapNode.attribute("tilewidth").as_uint();
    tileHeight   = mapNode.attribute("tileheight").as_uint();
    // int firstGid = mapNode.child("tileset").attribute("firstgid").as_int();
    std::map<int, std::string> gidStartToTileset;
    for (pugi::xml_node tilesetNode : mapNode.children("tileset")) {
        int firstGid = tilesetNode.attribute("firstgid").as_int();
        std::string source = tilesetNode.attribute("source").as_string(); // optional
        std::string name   = tilesetNode.attribute("name").as_string();   // fallback
        // std::string tilesetName = !source.empty() ? source : name;
        std::string tilesetName;
        if (!source.empty()) {
            // Lấy tên không có đuôi ".tsx" nếu có
            size_t lastSlash = source.find_last_of("/\\");
            size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
            size_t end = source.find_last_of('.');
            tilesetName = source.substr(start, end - start);
        } else {
            tilesetName = name;
        }
        gidStartToTileset[firstGid] = tilesetName;
    }

    // Chuẩn bị map GID → tên tileset tương ứng
    std::vector<int> gids;
    for (const auto& [gid, _] : gidStartToTileset) gids.push_back(gid);
    std::sort(gids.begin(), gids.end());

    for (size_t i = 0; i < gids.size(); ++i) {
        int start = gids[i];
        constexpr int MAX_GID_PER_TILESET = 10000;
        int  end = (i + 1 < gids.size()) ? gids[i + 1] - 1 : gids[i] + MAX_GID_PER_TILESET;
        std::string tilesetName = gidStartToTileset[start];
        for (int gid = start; gid <= end; ++gid) {
            m_gidToTilesetName[gid] = tilesetName;
        }
    }

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

        // while (std::getline(ss, tileIdStr, ',')) {
        //     tileIdStr.erase(std::remove_if(tileIdStr.begin(), tileIdStr.end(), ::isspace), tileIdStr.end());
        //     if (!tileIdStr.empty()) {
        //         layerTileIds.push_back(std::stoi(tileIdStr));
        //     }
        // }

        layerTileIds.reserve(mapWidth * mapHeight); // Tránh resize liên tục

        std::string line;
        while (std::getline(ss, line, ',')) {
            int id = std::atoi(line.c_str()); // nhanh hơn std::stoi với dữ liệu tin cậy
            layerTileIds.push_back(id);
        }
        
        // Kiểm tra lại kích thước cho chắc chắn
        if (layerTileIds.size() != mapWidth * mapHeight) {
            std::cerr << "Layer '" << layerNode.attribute("name").as_string() 
                      << "' data size does not match map dimensions!" << '\n';
            continue; // Bỏ qua layer bị lỗi
        }

        // Tạo VertexArray cho layer này (phần này giữ nguyên)
        // sf::VertexArray vertices;
        // vertices.setPrimitiveType(sf::Quads);
        // vertices.resize(mapWidth * mapHeight * 4);

        std::unordered_map<std::string, int> tilesetNameToFirstGid;
        for (const auto& [gid, name] : gidStartToTileset) {
            tilesetNameToFirstGid[name] = gid;
        }

        std::unordered_map<std::string, sf::VertexArray> tilesetToVertices;
        for (unsigned int i = 0; i < mapWidth; ++i) {
            for (unsigned int j = 0; j < mapHeight; ++j) {
                int tileGid = layerTileIds[i + j * mapWidth];
                if (tileGid == 0) continue;

                // int localTileId = tileGid - firstGid;
                // int tu = localTileId % (m_tilesetTexture.getSize().x / tileWidth);
                // int tv = localTileId / (m_tilesetTexture.getSize().x / tileWidth);
                if (m_gidToTilesetName.count(tileGid) == 0) continue; // không tìm được tileset

                std::string tilesetName = m_gidToTilesetName[tileGid];
                // const sf::Texture& tex = m_tilesetTextures.at(tilesetName);
                auto texIt = m_tilesetTextures.find(tilesetName);
                if (texIt == m_tilesetTextures.end()) continue;
                const sf::Texture& tex = texIt->second;

                // Tìm firstGid tương ứng tileset này:
                // int firstGidForTileset = -1;
                // for (const auto& [gid, name] : gidStartToTileset) {
                //     if (name == tilesetName) {
                //         firstGidForTileset = gid;
                //         break;
                //     }
                // }
                // if (firstGidForTileset < 0) continue;

                int firstGidForTileset = tilesetNameToFirstGid.at(tilesetName);

                int localTileId = tileGid - firstGidForTileset;
                int tilesPerRow = tex.getSize().x / tileWidth;
                int tu = localTileId % tilesPerRow;
                int tv = localTileId / tilesPerRow;

                sf::VertexArray& va = tilesetToVertices[tilesetName];
                if (va.getVertexCount() == 0) {
                    va.setPrimitiveType(sf::Quads);
                }

                // sf::Vertex* quad = &vertices[(i + j * mapWidth) * 4];
                sf::Vertex quad[4];

                quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
                quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
                quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
                quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

                quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                va.append(quad[0]);
                va.append(quad[1]);
                va.append(quad[2]);
                va.append(quad[3]);
            }
        }
        // m_layersVertices.push_back(vertices);
        for (auto& [tilesetName, va] : tilesetToVertices) {
            m_layerData.emplace_back(tilesetName, std::move(va));
        }
    }
    
    for (pugi::xml_node groupNode : mapNode.children("group")) {
        std::string groupName = groupNode.attribute("name").as_string();
        for (pugi::xml_node objectGroupNode : groupNode.children("objectgroup")) {
            std::string objectGroupName = objectGroupNode.attribute("name").as_string();
            for (pugi::xml_node objectNode : objectGroupNode.children("object")) {
                float x      = objectNode.attribute("x").as_float();
                float y      = objectNode.attribute("y").as_float();
                float width  = objectNode.attribute("width").as_float();
                float height = objectNode.attribute("height").as_float();
                
                if (groupName == "Collision") {
                    if (objectGroupName == "CollisionObjects") {
                        m_collisionRects.emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Npc") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "npc_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_NPCRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() Npc\n";
                    }
                }
                else if (groupName == "Enemy") {
                    if (objectGroupName == "Bat Lv.1") {
                        m_enemyRects["Bat Lv.1"].emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Eye Lv.5") {
                        m_enemyRects["Eye Lv.5"].emplace_back(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() Enemy\n";
                    }
                }
                else {
                    std::cerr << "[Bug] - TileMap.cpp - load()\n";
                }
            }
        }
    }

    return true;
}

void TileMap::updateObjects() {
    for (sf::FloatRect& rect : m_collisionRects) {
        rect = getTransform().transformRect(rect);
    }
    
    for (auto& pair : m_enemyRects) {
        for (sf::FloatRect& rect : pair.second) {
            rect = getTransform().transformRect(rect);
        }
    }

    for (auto& pair : m_NPCRects) {
        pair.second = getTransform().transformRect(pair.second);
    }

    tileWidth  *= 2;
    tileHeight *= 2;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    // states.texture = &m_tilesetTexture;

    // for (const auto& layer : m_layersVertices) {
    //     target.draw(layer, states);
    // }

    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }

    sf::RectangleShape hitbox;
    hitbox.setOutlineColor(sf::Color::Cyan);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    for (const sf::FloatRect& rect : m_collisionRects) {
        hitbox.setSize(rect.getSize());
        hitbox.setPosition(rect.getPosition());
        target.draw(hitbox);
    }
    
    hitbox.setOutlineColor(sf::Color::Yellow);
    for (auto& pair : m_enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            hitbox.setSize(rect.getSize());
            hitbox.setPosition(rect.getPosition());
            target.draw(hitbox);
        }
    }

    hitbox.setOutlineColor(sf::Color::Green);
    for (auto& pair : m_NPCRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }
}

sf::Vector2u TileMap::getPixelSize() const {
    unsigned int width  = mapWidth  * tileWidth;
    unsigned int height = mapHeight * tileHeight;
    return sf::Vector2u(width, height);
}

void TileMap::drawMinimap(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    // states.texture = &m_tilesetTexture;

    // for (const auto& layer : m_layersVertices) {
    //     target.draw(layer, states);
    // }
    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }
}

const std::vector<sf::FloatRect>& TileMap::getCollisionRects() const {
    return m_collisionRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getNpcRects() const {
    return m_NPCRects;
}

const std::unordered_map<std::string, std::vector<sf::FloatRect>>& TileMap::getEnemyRects() const {
    return m_enemyRects;
}