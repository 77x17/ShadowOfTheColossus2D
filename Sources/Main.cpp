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

sf::Font Font::font;

std::unordered_map<std::string, std::unique_ptr<sf::Texture>> TextureManager::textures;

std::unordered_map<std::string, sf::SoundBuffer> SoundManager::buffers;
std::unordered_map<std::string, sf::Sound> SoundManager::sounds;
std::unordered_map<std::string, std::unique_ptr<sf::Music>> SoundManager::regionMusic;
std::string SoundManager::oldRegionMusic = std::string();
float SoundManager::normalVolume = 50.0f;
float SoundManager::fadeVolume   = 50.0f;

std::unordered_map<std::string, std::unique_ptr<sf::Shader>> EntityEffects::shaders;

void loadSprite() {
    TextureManager::load("playerSprite", "Sprites/player.png");
    TextureManager::load("playerShadow", "Sprites/playerShadow.png");
    TextureManager::load("arrow"       , "Sprites/arrow.png");

    TextureManager::load("alert"    , "Sprites/alert.png");
    TextureManager::load("batSprite", "Sprites/bat.png");
    TextureManager::load("batShadow", "Sprites/batShadow.png");
    TextureManager::load("batDead"  , "Sprites/batDead.png");
    
    TextureManager::load("eyeSprite", "Sprites/eye.png");
    TextureManager::load("eyeShadow", "Sprites/eyeShadow.png");
    TextureManager::load("eyeDead"  , "Sprites/eyeDead.png");
    TextureManager::load("fireball" , "Sprites/fireball.png");
}

NaturalEffects naturalEffects;

void loadShader() {
    EntityEffects::load("invincible", "Shaders/entityEffects.frag", {
        {"invincibleAmount", 0.2f},
        {"flashAmount", 0.5f},
        {"useBlackFlash", false},
    });

    EntityEffects::load("flash", "Shaders/entityEffects.frag", {
        {"invincibleAmount", 0.2f},
        {"flashAmount", 1.0f},
        {"useBlackFlash", false}
    });

    EntityEffects::load("blackFlash", "Shaders/entityEffects.frag", {
        {"invincibleAmount", 0.2f},
        {"flashAmount", 1.0f},
        {"useBlackFlash", true}
    });

    EntityEffects::load("glow", "Shaders/glowEffects.frag", {
        {"glowColor", sf::Glsl::Vec4(1.0f, 1.0f, 1.0f, 1.0f)}
    });

    naturalEffects.load("Shaders/naturalEffects.frag");
}

void loadSound() {
    SoundManager::loadSound("arrow"     , "Sounds/arrow.wav");
    SoundManager::loadSound("roll"      , "Sounds/roll.wav");
    SoundManager::loadSound("playerHurt", "Sounds/playerHurt.wav");
    SoundManager::loadSound("playerDie" , "Sounds/playerDie.wav");
    SoundManager::loadSound("enemyHurt" , "Sounds/enemyHurt.wav");
    SoundManager::loadSound("enemyDie"  , "Sounds/enemyDie.wav");
    SoundManager::loadSound("talk"      , "Sounds/talk.wav");
    SoundManager::loadSound("menuOpen"  , "Sounds/menu-open.wav");
    SoundManager::loadSound("menuClose" , "Sounds/menu-close.wav");
    SoundManager::loadSound("levelUp"   , "Sounds/levelUp.wav");
    // SoundManager::loadMusic("region0"   , "Sounds/Salted - Wynn OST - 05 Detlas Suburb.ogg");
    // SoundManager::loadMusic("region1"   , "Sounds/Salted - Wynn OST - 04 Gavel Journey.ogg");
    // SoundManager::setMusicVolume("region0", 50.0f);
    // SoundManager::setMusicVolume("region1", 50.0f);

    // SoundManager::loadMusic("regionSong", "E:/Code/TestingSFML/Wynn OST ogg/Salted - Wynn OST - 01 Introduction.ogg");
}

void loadMap(TileMap& map) {
    map.load("Maps/test.tmx", {
        {"overworld", "Maps/overworld.png"},
        {"overworld_grass", "Maps/overworld_grass.png"},
        {"CastleWalls", "Maps/CastleWalls.png"},
        {"medium_oak_tree_static", "Maps/medium_oak_tree_static.png"},
        {"big_oak_tree_static", "Maps/big_oak_tree_static.png"}
    });

    map.scale(2.f, 2.f);
    map.updateObjects();
}

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
        // Elder Thorne
        npcRects.at(0)
    );
    npcs.emplace_back(
        1,
        // Torren 
        npcRects.at(1)
    );
    npcs.emplace_back(
        2,
        // Mira 
        npcRects.at(2)
    );
    npcs.emplace_back(
        3,
        // Bren 
        npcRects.at(3)
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

    Font::font.loadFromFile("Fonts/Roboto_Mono.ttf");
    loadSprite();
    loadShader();
    loadSound();

    TileMap map;
    loadMap(map);

    std::vector<std::unique_ptr<Enemy>> enemies;
    loadEnemy(enemies, map.getEnemyRects());

    std::vector<Npc> npcs;
    loadNpc(npcs, map.getNpcRects());
    
    std::vector<Quest> quests;
    loadQuests(quests);

    sf::Vector2f PlayerTiles = sf::Vector2f(
        78, 
        110
    );
    Player player(PlayerTiles.x * TILE_SIZE, PlayerTiles.y * TILE_SIZE, 5000.0f, std::move(quests));
    UI ui;

    ui.generateMinimapTexture(map);

    std::cerr << "Finished loading\n";

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
    // Load texture từ file
    particleManager.loadTexture(ParticleType::Rain , "Maps/leaf.png");
    particleManager.loadTexture(ParticleType::Leaf , "Maps/leaf.png");
    particleManager.loadTexture(ParticleType::Cloud, "Maps/cloud.png");

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
        player.update(dt, window, map.getCollisionRects(), map.getRegionRects(), npcs);
        ui.update(dt, player, uiView.getSize());
        
        // Điều chỉnh camera theo player
        player.updateView(dt, view);
        
        for (auto& enemy : enemies) {
            enemy->update(dt, player, map.getCollisionRects());
        }

        map.update(dt);
        
        particleManager.update(dt, view);
        
        {
            sf::Vector2f playerScreenPos = sf::Vector2f(window.mapCoordsToPixel(player.getCenterPosition(), view));
            sf::Vector2f windowSize = sf::Vector2f(window.getSize());
            
            sf::Vector2f lightNorm = {
                playerScreenPos.x / windowSize.x,
                playerScreenPos.y / windowSize.y
            };
            
            float aspectRatio = windowSize.x / windowSize.y;
            
            naturalEffects.update(dt, player.getCollisionRegionID(), lightNorm, aspectRatio);
        }
        // --- [End] Update ---

        sceneTexture.clear(sf::Color::White);
        sceneTexture.setView(view);
        
        // --- [Begin] Draw layer 1 --- 
        sceneTexture.draw(map);
        for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            if (!particleManager.isCollisionWithCloud(enemy->getFloatRect())) {
                enemy->draw(sceneTexture);
            }
        }

        player.draw(sceneTexture);
        
        sceneTexture.draw(particleManager);
        // --- [End] Draw layer 1 ---

        // --- [Begin] Draw layer 2 ---
        for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            if (particleManager.isCollisionWithCloud(enemy->getFloatRect())) {
                enemy->drawWithShader(sceneTexture, EntityEffects::get("glow"));
            }
        }

        if (particleManager.isCollisionWithCloud(player.getFloatRect())) {
            player.drawWithShader(sceneTexture, EntityEffects::get("glow"));
        }
        // --- [End] Draw layer 2 ---
    
        sceneTexture.display(); 

        window.clear();
        window.setView(uiView);

        // --- [Begin] Add natural shader --- 
        sf::Sprite sceneSprite(sceneTexture.getTexture());
        if (naturalEffects.shouldApplyShader()) {
            window.draw(sceneSprite, naturalEffects.get());
        }
        else {
            window.draw(sceneSprite); 
        }
        // --- [End] Add natural shader ---

        // particleManager.drawScreen(window);
        window.setView(uiView);
        
        ui.draw(window);

        window.display();
    }

    return 0;
}