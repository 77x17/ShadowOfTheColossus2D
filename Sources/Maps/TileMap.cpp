#include "TileMap.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

TileMap::TilesetInfo TileMap::getTilesetInfoForGid(int gid) const {
    auto it = m_gidStartToTileset.upper_bound(gid);
    if (it != m_gidStartToTileset.begin()) {
        --it;
    } else {
        return {"", -1};
    }
    return {it->second, it->first};
}

bool TileMap::load(const std::string& tmxPath, const std::vector<std::pair<std::string, std::string>>& tilesets) {
    m_layersVertices.clear();
    m_tilesetTextures.clear();

    m_overlayLayerData.clear();
    m_overlayAnimatedTiles.clear();

    m_lights.clear();

    for (const auto& [tilesetName, tilesetPath] : tilesets) {
        auto texture = std::make_unique<sf::Texture>();;
        if (!texture->loadFromFile(tilesetPath)) {
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

    if (mapNode.attribute("infinite").as_bool()) {
        std::cerr << "Error: This loader only supports fixed-size maps. Please disable 'Infinite' in Tiled map properties." << '\n';
        return false;
    }

    mapWidth     = mapNode.attribute("width").as_uint();
    mapHeight    = mapNode.attribute("height").as_uint();
    tileWidth    = mapNode.attribute("tilewidth").as_uint();
    tileHeight   = mapNode.attribute("tileheight").as_uint();
    m_gidStartToTileset.clear();
    for (pugi::xml_node tilesetNode : mapNode.children("tileset")) {
        int firstGid = tilesetNode.attribute("firstgid").as_int();
        std::string source = tilesetNode.attribute("source").as_string(); 
        std::string name   = tilesetNode.attribute("name").as_string();  
        std::string tilesetName;
        if (!source.empty()) {
            size_t lastSlash = source.find_last_of("/\\");
            size_t start = (lastSlash == std::string::npos) ? 0 : lastSlash + 1;
            size_t end = source.find_last_of('.');
            tilesetName = source.substr(start, end - start);
        } else {
            tilesetName = name;
        }
        m_gidStartToTileset[firstGid] = tilesetName;
    }

    std::string tmxDirectory;
    size_t lastSlash = tmxPath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        tmxDirectory = tmxPath.substr(0, lastSlash + 1);
    }

    for (pugi::xml_node tilesetNodeInMap : doc.child("map").children("tileset")) {
        int firstGid = tilesetNodeInMap.attribute("firstgid").as_int();
        pugi::xml_attribute sourceAttr = tilesetNodeInMap.attribute("source");

        if (sourceAttr) {
            std::string tsxPath = tmxDirectory + sourceAttr.as_string();
            
            pugi::xml_document tsxDoc;
            if (!tsxDoc.load_file(tsxPath.c_str())) {
                std::cerr << "Failed to load external tileset file: " << tsxPath << std::endl;
                continue;
            }
            
            pugi::xml_node tsxTilesetNode = tsxDoc.child("tileset");
            for (pugi::xml_node tileNode : tsxTilesetNode.children("tile")) {
                int localId = tileNode.attribute("id").as_int();
                int globalId = firstGid + localId;

                pugi::xml_node animationNode = tileNode.child("animation");
                if (animationNode) {
                    TileMapAnimation anim;
                    for (pugi::xml_node frameNode : animationNode.children("frame")) {
                        int frameLocalId = frameNode.attribute("tileid").as_int();
                        float durationMs = frameNode.attribute("duration").as_float();
                        
                        anim.frames.emplace_back(firstGid + frameLocalId, durationMs / 1000.f);
                    }
                    if (!anim.frames.empty()) {
                        m_animations[globalId] = anim;
                    }
                }
            }

        } else {
            for (pugi::xml_node tileNode : tilesetNodeInMap.children("tile")) {
                int localId = tileNode.attribute("id").as_int();
                int globalId = firstGid + localId;

                pugi::xml_node animationNode = tileNode.child("animation");
                if (animationNode) {
                    TileMapAnimation anim;
                    for (pugi::xml_node frameNode : animationNode.children("frame")) {
                        int frameLocalId = frameNode.attribute("tileid").as_int();
                        float durationMs = frameNode.attribute("duration").as_float();
                        
                        anim.frames.emplace_back(firstGid + frameLocalId, durationMs / 1000.f);
                    }
                    if (!anim.frames.empty()) {
                        m_animations[globalId] = anim;
                    }
                }
            }
        }
    }

    for (pugi::xml_node layerNode : mapNode.children("layer")) {
        std::string layerName = layerNode.attribute("name").as_string();
        bool isOverlayLayer = (layerName == "Overlay");

        pugi::xml_node dataNode = layerNode.child("data");
        if (!dataNode || std::string(dataNode.attribute("encoding").as_string()) != "csv") {
            continue;
        }

        std::string csvData = dataNode.text().get();
        std::vector<int> layerTileIds;
        std::stringstream ss(csvData);
        std::string tileIdStr;

        layerTileIds.reserve(mapWidth * mapHeight);

        std::string line;
        while (std::getline(ss, line, ',')) {
            int id = std::atoi(line.c_str());
            layerTileIds.push_back(id);
        }
        
        if (layerTileIds.size() != mapWidth * mapHeight) {
            std::cerr << "Layer '" << layerNode.attribute("name").as_string() 
                      << "' data size does not match map dimensions!" << '\n';
            continue; 
        }

        std::unordered_map<std::string, sf::VertexArray> tilesetToVertices;
        for (unsigned int i = 0; i < mapWidth; ++i) {
            for (unsigned int j = 0; j < mapHeight; ++j) {
                int tileGid = layerTileIds[i + j * mapWidth];
                if (tileGid == 0) continue;
                
                TilesetInfo tsInfo = getTilesetInfoForGid(tileGid);
                if (tsInfo.name.empty()) continue; 

                auto animIt = m_animations.find(tileGid);
                if (animIt != m_animations.end()) {
                    auto texIt = m_tilesetTextures.find(tsInfo.name);
                    if (texIt == m_tilesetTextures.end()) {
                        std::cerr << "[Bug] - TileMap.cpp() load()\n";
                        continue;
                    }
                    const sf::Texture& tex = *texIt->second;

                    AnimatedTile animatedTile;
                    animatedTile.animationData = &animIt->second; // Trỏ tới dữ liệu animation
                    animatedTile.currentFrame = 0;
                    animatedTile.elapsedTime = 0.0f; 
                    animatedTile.vertices.setPrimitiveType(sf::Quads);
                    
                    sf::Vertex quad[4];
                    quad[0].position = sf::Vector2f(i * tileWidth, j * tileHeight);
                    quad[1].position = sf::Vector2f((i + 1) * tileWidth, j * tileHeight);
                    quad[2].position = sf::Vector2f((i + 1) * tileWidth, (j + 1) * tileHeight);
                    quad[3].position = sf::Vector2f(i * tileWidth, (j + 1) * tileHeight);

                    int initialFrameTileGid = animatedTile.animationData->frames[0].tileID;
                    TilesetInfo frameTsInfo = getTilesetInfoForGid(initialFrameTileGid); 
                    int localTileId = initialFrameTileGid - frameTsInfo.firstGid;

                    int tilesPerRow = 0;
                    if (tileWidth != 0) {
                        tilesPerRow = tex.getSize().x / tileWidth;
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load()\n";
                    }

                    int tu = 0, tv = 0;
                    if (tilesPerRow != 0) {
                        tu = localTileId % tilesPerRow;
                        tv = localTileId / tilesPerRow;
                    }
                    
                    quad[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
                    quad[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
                    quad[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
                    quad[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);

                    for (int k = 0; k < 4; ++k) animatedTile.vertices.append(quad[k]);
                    
                    if (isOverlayLayer) {
                        m_overlayAnimatedTiles.push_back(std::move(animatedTile));
                    } else {
                        m_animatedTiles.push_back(std::move(animatedTile));
                    }
                } 
                else {
                    auto texIt = m_tilesetTextures.find(tsInfo.name);
                    if (texIt == m_tilesetTextures.end()) {
                        std::cerr << "[Bug] - TileMap.cpp() load()\n";
                        continue;
                    }
                    const sf::Texture& tex = *texIt->second;

                    int localTileId = tileGid - tsInfo.firstGid;
                    int tilesPerRow = 0;
                    if (tileWidth != 0) {
                        tilesPerRow = tex.getSize().x / tileWidth;
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load()\n";
                    }

                    int tu = 0, tv = 0;
                    if (tilesPerRow != 0) {
                        tu = localTileId % tilesPerRow;
                        tv = localTileId / tilesPerRow;
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load()\n";
                    }

                    sf::VertexArray& va = tilesetToVertices[tsInfo.name];
                    if (va.getVertexCount() == 0) {
                        va.setPrimitiveType(sf::Quads);
                    }

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
        }
        for (auto& [tilesetName, va] : tilesetToVertices) {
            if (isOverlayLayer) {
                m_overlayLayerData.emplace_back(tilesetName, std::move(va));
            } else {
                m_layerData.emplace_back(tilesetName, std::move(va));
            }
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
                    else if (objectGroupName == "Region") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "region_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_RegionRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else if (objectGroupName == "BossAltar") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "bossAltar_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_bossAltarRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() - Collision\n";
                    }
                }
                else if (groupName == "Npc") {
                    if (objectGroupName == "QuestNpc") {
                        auto prop = objectNode.child("properties").find_child_by_attribute("property", "name", "npc_id");
                        int id = -1;
                        if (prop) {
                            id = prop.attribute("value").as_int();
                        }
                        m_questNpcRects[id] = sf::FloatRect(x, y, width, height);
                    }
                    else if (objectGroupName == "MerchantNpc") {
                        m_merchantNpcRects.emplace_back(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() - Npc\n";
                    }
                }
                else if (groupName == "Enemy") {
                    if (objectGroupName == "Bat Lv.1") {
                        m_enemyRects["Bat Lv.1"].emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Vampire Bat Lv.3") {
                        m_enemyRects["Vampire Bat Lv.3"].emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Eye Lv.5") {
                        m_enemyRects["Eye Lv.5"].emplace_back(x, y, width, height);
                    }
                    else if (objectGroupName == "Slime Lv.5") {
                        m_enemyRects["Slime Lv.5"].emplace_back(x, y, width, height);
                    }
                    else {
                        std::cerr << "[Bug] - TileMap.cpp - load() - Enemy\n";
                    }
                }
                else if (groupName == "Light") {
                    m_lights.emplace_back(x, y);
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

    for (auto& pair : m_bossAltarRects) {
        pair.second = getTransform().transformRect(pair.second);
    }
    
    for (auto& pair : m_questNpcRects) {
        pair.second = getTransform().transformRect(pair.second);
    }

    for (sf::FloatRect& rect : m_merchantNpcRects) {
        rect = getTransform().transformRect(rect);
    }

    for (auto& pair : m_RegionRects) {
        pair.second = getTransform().transformRect(pair.second);
    }

    for (sf::Vector2f& lightPosition : m_lights) {
        lightPosition = getTransform().transformPoint(lightPosition);
    }
}

sf::FloatRect TileMap::getGlobalBounds() const {
    // 1. Lấy kích thước gốc của map (kích thước cục bộ)
    float width  = static_cast<float>(mapWidth * tileWidth);
    float height = static_cast<float>(mapHeight * tileHeight);
    sf::FloatRect localBounds(0.f, 0.f, width, height);

    // 2. Lấy ma trận transform hiện tại của đối tượng
    const sf::Transform& transform = getTransform();

    // 3. Áp dụng ma trận transform lên khung chữ nhật cục bộ để có được khung bao toàn cục
    return transform.transformRect(localBounds);
}

sf::Vector2u TileMap::getPixelSize() const {
    // Lấy khung chữ nhật bao quanh toàn cục đã tính toán các phép biến đổi
    sf::FloatRect bounds = getGlobalBounds();

    // Trả về kích thước của khung đó
    return sf::Vector2u(
        static_cast<unsigned int>(bounds.width),
        static_cast<unsigned int>(bounds.height)
    );
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &*m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }

    for (const auto& tile : m_animatedTiles) {
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        states.texture = &*m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }

    // sf::RectangleShape hitbox;
    // hitbox.setOutlineThickness(1.f);
    // hitbox.setFillColor(sf::Color::Transparent);
    
    // hitbox.setOutlineColor(sf::Color::Cyan);
    // for (const sf::FloatRect& rect : m_collisionRects) {
    //     hitbox.setSize(rect.getSize());
    //     hitbox.setPosition(rect.getPosition());
    //     target.draw(hitbox);
    // }
    
    // hitbox.setOutlineColor(sf::Color::Yellow);
    // for (auto& pair : m_enemyRects) {
    //     for (const sf::FloatRect& rect : pair.second) {
    //         hitbox.setSize(rect.getSize());
    //         hitbox.setPosition(rect.getPosition());
    //         target.draw(hitbox);
    //     }
    // }

    // hitbox.setOutlineColor(sf::Color::Magenta);
    // for (auto& pair : m_RegionRects) {
    //     hitbox.setSize(pair.second.getSize());
    //     hitbox.setPosition(pair.second.getPosition());
    //     target.draw(hitbox);
    // }
}

void TileMap::drawOverlay(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& [tilesetName, layer] : m_overlayLayerData) {
        states.texture = &*m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }

    for (const auto& tile : m_overlayAnimatedTiles) {
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        states.texture = &*m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }
}


void TileMap::drawMinimap(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const auto& [tilesetName, layer] : m_layerData) {
        states.texture = &*m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }
    for (const auto& tile : m_animatedTiles) {
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        states.texture = &*m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }

    for (const auto& [tilesetName, layer] : m_overlayLayerData) {
        states.texture = &*m_tilesetTextures.at(tilesetName);
        target.draw(layer, states);
    }
    for (const auto& tile : m_overlayAnimatedTiles) {
        int currentFrameGid = tile.animationData->frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;
        states.texture = &*m_tilesetTextures.at(tsInfo.name);
        target.draw(tile.vertices, states);
    }

    sf::RectangleShape hitbox;
    hitbox.setOutlineThickness(5.f);
    hitbox.setFillColor(sf::Color::Transparent);

    // hitbox.setOutlineColor(sf::Color::Cyan);
    // for (const sf::FloatRect& rect : m_collisionRects) {
    //     hitbox.setSize(rect.getSize());
    //     hitbox.setPosition(rect.getPosition());
    //     target.draw(hitbox);
    // }
    
    // hitbox.setOutlineColor(sf::Color::Yellow);
    // for (auto& pair : m_enemyRects) {
    //     for (const sf::FloatRect& rect : pair.second) {
    //         hitbox.setSize(rect.getSize());
    //         hitbox.setPosition(rect.getPosition());
    //         target.draw(hitbox);
    //     }
    // }

    hitbox.setOutlineColor(sf::Color::White);
    hitbox.setFillColor(sf::Color::White);
    for (auto& pair : m_bossAltarRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }

    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setFillColor(sf::Color::Green);
    for (auto& pair : m_questNpcRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }

    hitbox.setOutlineColor(sf::Color::Blue);
    hitbox.setFillColor(sf::Color::Blue);
    for (const sf::FloatRect& rect : m_merchantNpcRects) {
        hitbox.setSize(rect.getSize());
        hitbox.setPosition(rect.getPosition());
        target.draw(hitbox);
    }

    hitbox.setOutlineColor(sf::Color::Magenta);
    hitbox.setFillColor(sf::Color::Transparent);
    for (auto& pair : m_RegionRects) {
        hitbox.setSize(pair.second.getSize());
        hitbox.setPosition(pair.second.getPosition());
        target.draw(hitbox);
    }
}

const std::vector<sf::FloatRect>& TileMap::getCollisionRects() const {
    return m_collisionRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getBossAltarRects() const {
    return m_bossAltarRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getQuestNpcRects() const {
    return m_questNpcRects;
}

const std::vector<sf::FloatRect>& TileMap::getMerchantNpcRects() const {
    return m_merchantNpcRects;
}

const std::unordered_map<std::string, std::vector<sf::FloatRect>>& TileMap::getEnemyRects() const {
    return m_enemyRects;
}

const std::unordered_map<int, sf::FloatRect>& TileMap::getRegionRects() const {
    return m_RegionRects;
}

const std::vector<sf::Vector2f>& TileMap::getLights() const {
    return m_lights;
}

void TileMap::update(const float& dt) {
    for (auto& tile : m_animatedTiles) {
        tile.elapsedTime += dt;
        
        const auto& frames = tile.animationData->frames;
        if (frames.empty()) {
            std::cerr << "[Bug] - TileMap.cpp - update()\n";
            continue;
        }
        if (tile.elapsedTime >= frames[tile.currentFrame].duration) {
            tile.elapsedTime -= frames[tile.currentFrame].duration;
            tile.currentFrame = (tile.currentFrame + 1) % frames.size();

            int nextFrameGid = frames[tile.currentFrame].tileID;
            
            TilesetInfo tsInfo = getTilesetInfoForGid(nextFrameGid);
            if (tsInfo.name.empty()) continue; // Bỏ qua nếu có lỗi

            auto texIt = m_tilesetTextures.find(tsInfo.name);
            if (texIt == m_tilesetTextures.end()) {
                std::cerr << "[Bug] - TileMap.cpp() load()\n";
                continue;
            }
            const sf::Texture& tex = *texIt->second;

            int localTileId = nextFrameGid - tsInfo.firstGid;
            int tilesPerRow = 0;
            if (tileWidth != 0) {
                tilesPerRow = tex.getSize().x / tileWidth;
            }
            else {
                std::cerr << "[Bug] - TileMap.cpp - load()\n";
            }
            
            int tu = 0, tv = 0;
            if (tilesPerRow != 0) {
                tu = localTileId % tilesPerRow;
                tv = localTileId / tilesPerRow;
            }
            else {
                std::cerr << "[Bug] - TileMap.cpp - update()\n";
            }

            tile.vertices[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
            tile.vertices[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
            tile.vertices[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
            tile.vertices[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
        }
    }

    for (auto& tile : m_overlayAnimatedTiles) {
        tile.elapsedTime += dt;
        
        const auto& frames = tile.animationData->frames;
        if (frames.empty()) {
            std::cerr << "[Bug] - TileMap.cpp - update()\n";
            continue;
        }
        
        while (tile.elapsedTime >= frames[tile.currentFrame].duration) {
            tile.elapsedTime -= frames[tile.currentFrame].duration;
            tile.currentFrame = (tile.currentFrame + 1) % frames.size();
        }

        int currentFrameGid = frames[tile.currentFrame].tileID;
        TilesetInfo tsInfo = getTilesetInfoForGid(currentFrameGid);
        if (tsInfo.name.empty()) continue;

        auto texIt = m_tilesetTextures.find(tsInfo.name);
        if (texIt == m_tilesetTextures.end()) continue;
        const sf::Texture& tex = *texIt->second;

        int localTileId = currentFrameGid - tsInfo.firstGid;
        if (tileWidth == 0) {
            std::cerr << "[Bug] - TileMap.cpp - update()\n";
        }
        int tilesPerRow = tex.getSize().x / tileWidth;
        if (tilesPerRow == 0) {
            std::cerr << "[Bug] - TileMap.cpp - update()\n";
            continue;
        }
        int tu = localTileId % tilesPerRow;
        int tv = localTileId / tilesPerRow;

        tile.vertices[0].texCoords = sf::Vector2f(tu * tileWidth, tv * tileHeight);
        tile.vertices[1].texCoords = sf::Vector2f((tu + 1) * tileWidth, tv * tileHeight);
        tile.vertices[2].texCoords = sf::Vector2f((tu + 1) * tileWidth, (tv + 1) * tileHeight);
        tile.vertices[3].texCoords = sf::Vector2f(tu * tileWidth, (tv + 1) * tileHeight);
    }

    for (auto& pair : m_overlayLayerData) {
        sf::VertexArray& va = pair.second;
        for (std::size_t i = 0; i < va.getVertexCount(); ++i) {
            va[i].color = sf::Color::White;
        }
    }
    for (auto& tile : m_overlayAnimatedTiles) {
        for (std::size_t i = 0; i < 4; ++i) { // Mỗi tile động là 1 quad (4 đỉnh)
            tile.vertices[i].color = sf::Color::White;
        }
    }
}

void TileMap::updateOverlayTransparency(const sf::FloatRect& targetBounds) {
    const sf::Color TransparentColor(255, 255, 255, 150);

    const int fadeRadius = 2;
    const float influenceWidth  = static_cast<float>((2 * fadeRadius + 1) * tileWidth);
    const float influenceHeight = static_cast<float>((2 * fadeRadius + 1) * tileHeight);

    for (auto& pair : m_overlayLayerData) {
        sf::VertexArray& va = pair.second;
        for (std::size_t i = 0; i < va.getVertexCount(); i += 4) {
            sf::Vector2f tilePosition = va[i].position;
            float influenceLeft = tilePosition.x - (fadeRadius * tileWidth);
            float influenceTop  = tilePosition.y - (fadeRadius * tileHeight);
            sf::FloatRect influenceRect(influenceLeft, influenceTop, influenceWidth, influenceHeight);
            sf::FloatRect transformedInfluenceRect = getTransform().transformRect(influenceRect);
            if (targetBounds.intersects(transformedInfluenceRect)) {
                va[i].color = TransparentColor;
                va[i + 1].color = TransparentColor;
                va[i + 2].color = TransparentColor;
                va[i + 3].color = TransparentColor;
            }
        }
    }

    for (auto& tile : m_overlayAnimatedTiles) {
        sf::Vector2f tilePosition = tile.vertices[0].position;
        float influenceLeft = tilePosition.x - (fadeRadius * tileWidth);
        float influenceTop  = tilePosition.y - (fadeRadius * tileHeight);
        sf::FloatRect influenceRect(influenceLeft, influenceTop, influenceWidth, influenceHeight);
        sf::FloatRect transformedInfluenceRect = getTransform().transformRect(influenceRect);
        if (targetBounds.intersects(transformedInfluenceRect)) {
            tile.vertices[0].color = TransparentColor;
            tile.vertices[1].color = TransparentColor;
            tile.vertices[2].color = TransparentColor;
            tile.vertices[3].color = TransparentColor;
        }
    }
}

void TileMap::loadMap() {
    load("Maps/test.tmx", {
        {"overworld", "Maps/overworld.png"},
        {"overworld_grass", "Maps/overworld_grass.png"},
        {"CastleWalls", "Maps/CastleWalls.png"},
        {"medium_oak_tree_static", "Maps/medium_oak_tree_static.png"},
        {"big_oak_tree_static", "Maps/big_oak_tree_static.png"},
        {"Swamp", "Maps/Swamp.png"},
        {"DesertTilemapBlankBackground", "Maps/DesertTilemapBlankBackground.png"}
    });

    scale(2.f, 2.f);
    updateObjects();
}