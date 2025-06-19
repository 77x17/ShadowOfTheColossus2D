#include <SFML/Graphics.hpp>
#include <memory>
#include <windows.h>

#include "Constants.hpp"

#include "Font.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

#include "Player.hpp"
#include "UI.hpp"

#include "Enemy.hpp"
#include "Bat.hpp"
#include "Eye.hpp"

#include "Quest.hpp"
#include "KillMonsterObjective.hpp"
#include "FinishObjective.hpp"
#include "TalkObjective.hpp"
#include "ExploreObjective.hpp"
#include "Npc.hpp"

#include "EntityEffects.hpp"
#include "NaturalEffects.hpp"
#include "ParticleManager.hpp"
#include "TextureManager.hpp"

#include "Clock.hpp"

#include "InventoryUI.hpp"

sf::Font Font::font;

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> TextureManager::textures;

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::unordered_map<std::string, sf::Sound> SoundManager::sounds;
std::unordered_map<std::string, std::unique_ptr<sf::Music>> SoundManager::regionMusic;
std::string SoundManager::oldRegionMusic = std::string();
float SoundManager::normalVolume = 100.0f;
float SoundManager::fadeVolume   = 100.0f;

std::unordered_map<std::string, std::unique_ptr<sf::Shader>> EntityEffects::shaders;

void loadEnemy(std::vector<std::unique_ptr<Enemy>>& enemies, const std::unordered_map<std::string, std::vector<sf::FloatRect>>& enemyRects) {
    for (const auto& pair : enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            if (pair.first == "Bat Lv.1") {
                enemies.push_back(std::make_unique<Bat>(rect.getPosition().x, rect.getPosition().y));
            }
            else if (pair.first == "Eye Lv.5") {
                enemies.push_back(std::make_unique<Eye>(rect.getPosition().x, rect.getPosition().y));
            }
            else {
                std::cerr << "[Bug] - Main.cpp - loadEnemy()\n";
            }
        }
    }
}

void loadNpc(std::vector<Npc>& npcs, const std::unordered_map<int, sf::FloatRect>& npcRects) {
    npcs.emplace_back(
        0,
        npcRects.at(0),
        "Elder Thorne",
        "npc_00"
    );
    npcs.emplace_back(
        1,
        npcRects.at(1),
        "Torren",
        "npc_01"
    );
    npcs.emplace_back(
        2,
        npcRects.at(2),
        "Mira",
        "npc_02"
    );
    npcs.emplace_back(
        3,
        npcRects.at(3),
        "Bren",
        "npc_03"
    );
}

void loadQuests(std::vector<Quest>& quests) {
    {
        quests.push_back(Quest("Explore strange paths", 40));
        quests.back().addRequiredLevel(1);

        quests.back().addNpcID      (0, 0);
        quests.back().addDialogue   (0, "[1/4] The forest has changed. It no longer feels safe");
        quests.back().addDialogue   (0, "[2/4] Long ago, we sealed something deep beyond those hills");
        quests.back().addDialogue   (0, "[3/4] But now... whispers return, and beasts creep closer each night");
        quests.back().addDialogue   (0, "[4/4] You must go, find the truth. We're counting on you");
        quests.back().addDescription(0, "Come see the road behind the wooden bridge");
        quests.back().addObjective  (0, std::make_shared<ExploreObjective>(1));
        
        quests.back().addNpcID      (1, -1);
        quests.back().addDialogue   (1, std::string());
        quests.back().addDescription(1, "Return back to Elder Throne");
        quests.back().addObjective  (1, std::make_shared<TalkObjective>(0));

        quests.back().addNpcID      (2, 0);
        quests.back().addDialogue   (2, "[1/3] Oh, you are back");
        quests.back().addDialogue   (2, "[2/3] This place used to be very peaceful");
        quests.back().addDialogue   (2, "[3/3] Talk to Torren to prepare for the journey");
        quests.back().addDescription(2, "Return back to Elder Throne");
        quests.back().addObjective  (2, std::make_shared<TalkObjective>(0));

        quests.back().addNpcID      (3, -1);
        quests.back().addDialogue   (3, std::string());
        quests.back().addDescription(3, "Find Torren");
        quests.back().addObjective  (3, std::make_shared<TalkObjective>(1));
        
        quests.back().addNpcID      (4, 1);
        quests.back().addDialogue   (4, "[1/6] Oh, Elder Thorne told you to come here?");
        quests.back().addDialogue   (4, "[2/6] You sure about leaving? It's dangerous out there");
        quests.back().addDialogue   (4, "[3/6] I've seen strange tracks near the river, not animal ones");
        quests.back().addDialogue   (4, "[4/6] If you're going, take this bow. Just in case");
        quests.back().addDialogue   (4, "[5/6] Press [Space] to use that bow");
        quests.back().addDialogue   (4, "[6/6] Now, as an archer, help us defeat the monsters around here");
        quests.back().addDescription(4, "Find Torren");
        quests.back().addObjective  (4, std::make_shared<TalkObjective>(1));
        
        quests.back().addNpcID      (5, -1);
        quests.back().addDialogue   (5, std::string());
        quests.back().addDescription(5, "Help the villages defeat the monsters");
        quests.back().addObjective  (5, std::make_shared<KillMonsterObjective>("Bat Lv.1", 5));
        quests.back().addObjective  (5, std::make_shared<KillMonsterObjective>("Eye Lv.5", 2));

        quests.back().addNpcID      (6, -1);
        quests.back().addDialogue   (6, std::string());
        quests.back().addDescription(6, "Return back to Torren");
        quests.back().addObjective  (6, std::make_shared<TalkObjective>(1));

        quests.back().addNpcID      (7, 1);
        quests.back().addDialogue   (7, "[1/3] Oh, you are back");
        quests.back().addDialogue   (7, "[2/3] You like the bow I gave you, no problem");
        quests.back().addDialogue   (7, "[3/3] I see, you are strong enough to write your journey");
        quests.back().addDescription(7, "Return back to Torren");
        quests.back().addObjective  (7, std::make_shared<TalkObjective>(1));
    }
    // {
    //     quests.push_back(Quest("Help the children", 100));
    //     quests.back().addRequiredLevel(2);

    //     quests.back().addNpcID      (stage, npcID);
    //     quests.back().addDialogue   (stage, "...");
    //     quests.back().addDescription(stage, "...");
    //     quests.back().addObjective  (stage, objective);
    // }
    {
        // quests.push_back(Quest("...", exp));
        // quests.back().addRequiredLevel(playerLevel);

        // quests.back().addNpcID      (stage, npcID);
        // quests.back().addDialogue   (stage, "...");
        // quests.back().addDescription(stage, "...");
        // quests.back().addObjective  (stage, objective);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project_H", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // [Begin] - Loading
    Font::font.loadFromFile("Assets/Fonts/Roboto_Mono.ttf");
    TextureManager::loadSprite();
    EntityEffects::loadShader();

    NaturalEffects naturalEffects;
    naturalEffects.load("Assets/Shaders/naturalEffects.frag");
    naturalEffects.loadSmartLightingShader("Assets/Shaders/smartLighting.frag");

    SoundManager::loadSound();

    TileMap map;
    map.loadMap();

    std::vector<std::unique_ptr<Enemy>> enemies;
    loadEnemy(enemies, map.getEnemyRects());

    std::vector<Npc> npcs;
    loadNpc(npcs, map.getNpcRects());
    
    std::vector<Quest> quests;
    loadQuests(quests);

    sf::Vector2f PlayerTiles = sf::Vector2f( 78, 110);
    Player player(PlayerTiles.x * TILE_SIZE, PlayerTiles.y * TILE_SIZE, 10.0f, std::move(quests));
    UI ui;
    ui.generateMinimapTexture(map);

    srand(static_cast<unsigned int>(time(0)));

    sf::Clock clock;
    bool      isMinimized  = false;
    bool      isFullscreen = false;
    sf::View  view         = window.getView();
    view.setCenter(PlayerTiles.x * TILE_SIZE, PlayerTiles.y * TILE_SIZE);
    sf::View  uiView       = window.getDefaultView();
    
    sf::RenderTexture sceneTexture;                     //  for shader
    sceneTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);   //  for shader

    ParticleManager particleManager;
    particleManager.loadTexture();

    Clock gameClock(6 * 60.0f);

    int playerLightID = naturalEffects.addLight(player.getPosition(), 350.0f, sf::Vector3f(1.0f, 1.0f, 1.0f), 1.0f, 1);
    for (const sf::Vector2f lightPosition : map.getLights()) {
        naturalEffects.addLight(lightPosition);
    }

    Item* bow_00    = new Bow("Old Bow", "bow_00", 1.0f, 1);
    Item* bow_01    = new Bow("Wooden Bow", "bow_00", 2.5f, 1);
    Item* helmet_00 = new Helmet("Old Helmet", "helmet_00", 2.0f, 1);
    Item* helmet_01 = new Helmet("Copper Helmet", "helmet_00", 4.0f, 1);

    player.addItem(bow_00);
    player.addItem(bow_01);
    player.addItem(helmet_00);
    player.addItem(helmet_01);

    InventoryUI inventoryUI(static_cast<sf::Vector2f>(window.getSize()), player);
    // [End] - Loading

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::LostFocus) {
                isMinimized = true;

                if (isFullscreen) {
                    HWND hwnd = window.getSystemHandle(); // lấy HWND của cửa sổ
                    ShowWindow(hwnd, SW_MINIMIZE);        // minimize cửa sổ
                }
            }
            else if (event.type == sf::Event::GainedFocus) {
                isMinimized = false;
            }
            else if (event.type == sf::Event::Resized) {
                float aspectRatio = static_cast<float>(event.size.width) / event.size.height;
                
                view = window.getView();
                view.setSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);  // giữ chiều cao cố định
                view.setCenter(player.getCenterPosition());
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::F11) {
                    isFullscreen = !isFullscreen;

                    window.close();
                    if (isFullscreen) {
                        window.create(sf::VideoMode::getDesktopMode(), "Project_H", sf::Style::Fullscreen);
                        sceneTexture.create(window.getSize().x, window.getSize().y);
                    } else {
                        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project_H", sf::Style::Close);
                        sceneTexture.create(window.getSize().x, window.getSize().y);
                    }
                    // window.setFramerateLimit(60);
                    window.setVerticalSyncEnabled(true);
                    
                    sf::Vector2u windowSize = window.getSize();
                    float aspectRatio = static_cast<float>(windowSize.x) / windowSize.y;

                    view = window.getView();
                    view.setSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);  
                    view.setCenter(player.getCenterPosition());

                    uiView = window.getDefaultView();

                    inventoryUI.updatePosition(static_cast<sf::Vector2f>(window.getSize()));
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    ui.updateQuestsBox();
                }
                else if (event.key.code == sf::Keyboard::Hyphen) {
                    ui.updateMinimapBoxSize(false);
                }
                else if (event.key.code == sf::Keyboard::Equal) {
                    ui.updateMinimapBoxSize(true);
                }
                else if (event.key.code == sf::Keyboard::M) {
                    ui.openMap(uiView.getSize());
                }
                else if (event.key.code == sf::Keyboard::E) {
                    inventoryUI.toggle();
                }
            }
            else if (inventoryUI.isVisible() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                inventoryUI.handleClick(mousePos);
            }
            else if (inventoryUI.isVisible() && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                inventoryUI.handleRelease(mousePos, player);
            }
        }

        if (dt <= 0 || dt > 1.0f) {
            std::cerr << "[Bug] - Main.cpp - main(): " << dt << "\n";
            dt = 0.016f; // Fallback to 60fps
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }
        
        // --- [Begin] update --- 
        gameClock.update(dt);

        player.update(dt, window, map.getCollisionRects(), map.getRegionRects(), npcs);
        ui.update(dt, player, uiView.getSize(), gameClock);
        
        // Điều chỉnh camera theo player
        player.updateView(dt, view);
        
        for (auto& enemy : enemies) {
            enemy->update(dt, player, map.getCollisionRects());
        }

        for (auto& npc : npcs) {
            npc.update();
        }
        
        map.update(dt);
        map.updateOverlayTransparency(player.getFloatRect());
        for (const auto& enemy : enemies) {
            map.updateOverlayTransparency(enemy->getFloatRect());
        }
        
        particleManager.update(dt, view);
        particleManager.isCollisionWithCloud(player.getFloatRect());
        for (const auto& enemy : enemies) {
            particleManager.isCollisionWithCloud(enemy->getFloatRect());
        }
        particleManager.isCollisionWithRain(player.getCollisionRegionID());
        
        {
            sf::Vector2f playerScreenPos = sf::Vector2f(window.mapCoordsToPixel(player.getCenterPosition(), view));
            sf::Vector2f windowSize = sf::Vector2f(window.getSize());
            
            sf::Vector2f lightNorm = {
                playerScreenPos.x / windowSize.x,
                playerScreenPos.y / windowSize.y
            };
            
            float aspectRatio = windowSize.x / windowSize.y;
            
            naturalEffects.update(dt, player.getCollisionRegionID(), lightNorm, aspectRatio, gameClock);
            naturalEffects.updateLightPosition(playerLightID, player.getCenterPosition()); 
            naturalEffects.updateSmartLighting(player.getCenterPosition(), view);
        }

        if (inventoryUI.isVisible() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            inventoryUI.updateDrag(mousePos);
        }

        if (inventoryUI.isVisible()) {
            inventoryUI.updateStats(player);
        }

        // --- [End] Update ---

        sceneTexture.clear(sf::Color::White);
        sceneTexture.setView(view);
        
        // --- [Begin] Draw layer 1 --- 
        sceneTexture.draw(map);

        for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            enemy->draw(sceneTexture);
        }

        for (auto& npc : npcs) {
            npc.draw(sceneTexture);
        }

        player.draw(sceneTexture);
        
        map.drawOverlay(sceneTexture);

        sceneTexture.draw(particleManager);
        // --- [End] Draw layer 1 ---

        // --- [Begin] Draw layer 2 ---
        sceneTexture.setView(uiView);
        particleManager.drawScreen(sceneTexture);

        sceneTexture.setView(view);
        player.drawInteractText(sceneTexture);
        // --- [End] Draw layer 2 ---

        sceneTexture.display(); 

        window.clear();
        window.setView(uiView);

        // --- [Begin] Add natural shader --- 
        sf::Sprite sceneSprite(sceneTexture.getTexture());
        if (naturalEffects.shouldApplySmartLighting()) {
            window.draw(sceneSprite, naturalEffects.getSmartLightingShader());
        }
        else if (naturalEffects.shouldApplyShader()) {
            window.draw(sceneSprite, naturalEffects.get());
        }
        else {
            window.draw(sceneSprite); 
        }
        // --- [End] Add natural shader ---
        
        ui.draw(window);

        if (inventoryUI.isVisible()) {
            inventoryUI.draw(window);
        }

        window.display();
    }

    return 0;
}