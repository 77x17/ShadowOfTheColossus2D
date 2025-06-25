#include <SFML/Graphics.hpp>
#include <windows.h>

#include "Constants.hpp"
#include "Font.hpp"
#include "TileMap.hpp"
#include "Clock.hpp"

// --- [Begin] - Database ---
#include "ItemDatabase.hpp"
// --- [End] ---

// --- [Begin] - Manager ---
#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "ParticleManager.hpp"
#include "EnemyManager.hpp"
#include "NpcManager.hpp"
#include "QuestManager.hpp"
#include "ItemManager.hpp"
// --- [End] ---

// --- [Begin] - Entities ---
#include "Player.hpp"
#include "Item.hpp"
// --- [End] ---

// --- [Begin] - UI ---
#include "UI.hpp"
#include "InventoryUI.hpp"
#include "MerchantUI.hpp"
// --- [End] ---

#include "EntityEffects.hpp"
// #include "NaturalEffects.hpp"

#include "BossAltarManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project_H", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //  --- [Begin] - Loading ---
    Font::font.loadFromFile("Assets/Fonts/Roboto_Mono.ttf");
    TextureManager::loadSprites();
    SoundManager::loadSounds();
    EntityEffects::loadShaders();
    ItemDatabase::loadItems();

    // NaturalEffects naturalEffects;
    // naturalEffects.load("Assets/Shaders/naturalEffects.frag");
    // naturalEffects.loadSmartLightingShader("Assets/Shaders/smartLighting.frag");

    TileMap map;
    map.loadMap();

    EnemyManager enemies;
    enemies.loadEnemies(map.getEnemyRects());

    NpcManager npcs;
    npcs.loadNpcs(map);
    
    QuestManager quests;
    quests.loadQuests();

    sf::Vector2f playerPosition = sf::Vector2f( 78, 110) * static_cast<float>(TILE_SIZE);

    ItemManager items;
    items.addItem(playerPosition + sf::Vector2f(100.0f, 0), std::make_shared<Bow>   ("God Bow", "bow_00"      , 1, ItemRarity::Mythic, 100.0f));
    items.addItem(playerPosition + sf::Vector2f(300.0f, 0), std::make_shared<Helmet>("God Helmet", "helmet_00", 1, ItemRarity::Mythic, 100.0f));

    Player player(playerPosition, 10.0f, std::move(quests.getQuests()));
    
    player.addItem(ItemDatabase::get("Bat Orb", 10));
    player.addItem(ItemDatabase::get("Eye Orb", 10));

    srand(static_cast<unsigned int>(time(0)));

    sf::Clock clock;
    bool      isMinimized  = false;
    bool      isFullscreen = false;
    sf::View  view         = window.getView();
    view.setCenter(playerPosition);
    sf::View  uiView       = window.getDefaultView();
    
    sf::RenderTexture sceneTexture;                     //  for shader
    sceneTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);   //  for shader

    ParticleManager particleManager;
    particleManager.loadTexture();

    Clock gameClock(6 * 60.0f);

    // int playerLightID = naturalEffects.addLight(player.getPosition(), 350.0f, sf::Vector3f(1.0f, 1.0f, 1.0f), 1.0f, 1);
    // for (const sf::Vector2f lightPosition : map.getLights()) {
    //     naturalEffects.addLight(lightPosition);
    // }

    UI ui;
    ui.generateMinimapTexture(map);
    InventoryUI inventoryUI(static_cast<sf::Vector2f>(window.getSize()), player);
    MerchantUI  merchantUI (static_cast<sf::Vector2f>(window.getSize()), player);

    BossAltarManager bossAltars;
    bossAltars.loadBossAltars(map.getBossAltarRects());
    // --- [End] ---

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
                    merchantUI.updatePosition(static_cast<sf::Vector2f>(window.getSize()));
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
                inventoryUI.handleRelease(mousePos, player, items);
            }
            else if (merchantUI.isVisible() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                merchantUI.handleClick(mousePos);
            }
            else if (merchantUI.isVisible() && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                merchantUI.handleRelease(mousePos, player, items);
            }
            else if (inventoryUI.isVisible() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                inventoryUI.handleRightClick(mousePos);
            }
            else if (inventoryUI.isVisible() && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                inventoryUI.handleRelease(mousePos, player, items);
            }
            else if (merchantUI.isVisible() && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                merchantUI.handleRightClick(mousePos);
            }
            else if (merchantUI.isVisible() && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                merchantUI.handleRelease(mousePos, player, items);
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
        
        // --- [Begin] Update --- 
        gameClock.update(dt);

        npcs.handlePlayerInteraction(player);
        
        items.handlePlayerCollision(player);

        // Player collision Boss Altars
        bossAltars.handlePlayerInteraction(player);

        player.update(dt, window, map.getCollisionRects(), map.getRegionRects());
        
        ui.update(dt, player, uiView.getSize(), gameClock);
        
        player.updateView(dt, view);
        
        enemies.update(dt, player, map.getCollisionRects(), items);

        npcs.update(dt);

        items.update(dt);

        bossAltars.update(dt, enemies);
        
        map.update(dt);
        map.updateOverlayTransparency(player.getHitbox());
        for (const auto& enemy : enemies.getEnemies()) {
            map.updateOverlayTransparency(enemy->getHitbox());
        }
        for (const Item& item : items.getItems()) {
            map.updateOverlayTransparency(item.getHitbox());
        }
        
        particleManager.update(dt, view);
        particleManager.isCollisionWithCloud(player.getHitbox());
        for (const auto& enemy : enemies.getEnemies()) {
            particleManager.isCollisionWithCloud(enemy->getHitbox());
        }
        for (const Item& item : items.getItems()) {
            particleManager.isCollisionWithCloud(item.getHitbox());
        }
        particleManager.isCollisionWithRain(player.getCollisionRegionID());
        
        // {
        //     sf::Vector2f playerScreenPos = sf::Vector2f(window.mapCoordsToPixel(player.getCenterPosition(), view));
        //     sf::Vector2f windowSize = sf::Vector2f(window.getSize());
            
        //     sf::Vector2f lightNorm = {
        //         playerScreenPos.x / windowSize.x,
        //         playerScreenPos.y / windowSize.y
        //     };
            
        //     float aspectRatio = windowSize.x / windowSize.y;
            
        //     naturalEffects.update(dt, player.getCollisionRegionID(), lightNorm, aspectRatio, gameClock);
        //     naturalEffects.updateLightPosition(playerLightID, player.getCenterPosition()); 
        //     naturalEffects.updateSmartLighting(player.getCenterPosition(), view);
        // }

        if (inventoryUI.isVisible() && player.isAlive()) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                inventoryUI.updateDrag(mousePos);
            }
            else {
                inventoryUI.updateHover(mousePos);    
            }

            inventoryUI.updateStats(player);
            
            inventoryUI.updateAmount();
        }
        else {
            if (inventoryUI.isDrag()) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                inventoryUI.handleRelease(mousePos, player, items);
            }
        }

        if (merchantUI.isVisible()) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                merchantUI.updateDrag(mousePos);
            }
            else {
                merchantUI.updateHover(mousePos);    
            }

            merchantUI.updateAmount();
        }
        else {
            if (merchantUI.isDrag()) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                merchantUI.handleRelease(mousePos, player, items);
            }
        }

        // --- [End] ---

        sceneTexture.clear(sf::Color::White);
        sceneTexture.setView(view);
        
        // --- [Begin] Draw layer 1 --- 
        sceneTexture.draw(map);

        enemies.draw(sceneTexture, player);

        npcs.draw(sceneTexture);

        items.draw(sceneTexture);

        player.draw(sceneTexture);
        
        map.drawOverlay(sceneTexture);

        sceneTexture.draw(particleManager);
        // --- [End] ---

        // --- [Begin] Draw layer 2 ---
        sceneTexture.setView(uiView);
        particleManager.drawScreen(sceneTexture);
        
        sceneTexture.setView(view);
        npcs.drawInteractText(sceneTexture);
        
        bool merchantFlag = npcs.isInteractWithMerchant(inventoryUI.isVisible());
        if (merchantUI.isVisible() && merchantFlag == false) {
            merchantUI.isPayment(player);
        }
        merchantUI.setVisible(merchantFlag);

        bossAltars.drawInteractText(sceneTexture);
        // --- [End] ---

        sceneTexture.display(); 

        window.clear();
        window.setView(uiView);

        sf::Sprite sceneSprite(sceneTexture.getTexture());

        // --- [Begin] Add natural shader --- 
        // if (naturalEffects.shouldApplySmartLighting()) {
        //     window.draw(sceneSprite, naturalEffects.getSmartLightingShader());
        // }
        // else if (naturalEffects.shouldApplyShader()) {
        //     window.draw(sceneSprite, naturalEffects.get());
        // }
        // else {
        //     window.draw(sceneSprite); 
        // }
        // --- [End] --- 
        
        window.draw(sceneSprite); 
        
        ui.draw(window);

        if (inventoryUI.isVisible()) {
            inventoryUI.draw(window);
        }
        if (merchantUI.isVisible()) {
            merchantUI.draw(window);
        }

        window.display();
    }

    return 0;
}