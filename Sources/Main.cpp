#include <SFML/Graphics.hpp>
#include <memory>
#include <windows.h>

#include "Constants.hpp"

#include "Clock.hpp"

#include "Font.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

// --- [Begin] - Entities ---
#include "Enemy.hpp"
#include "Bat.hpp"
#include "BatBoss.hpp"
#include "Eye.hpp"
#include "Player.hpp"
#include "Item.hpp"
// --- [End] ---

// --- [Begin] - UI ---
#include "UI.hpp"
#include "InventoryUI.hpp"
#include "MerchantUI.hpp"
// --- [End] ---

#include "Quest.hpp"
#include "KillMonsterObjective.hpp"
#include "FinishObjective.hpp"
#include "TalkObjective.hpp"
#include "ExploreObjective.hpp"
#include "GiveItemObjective.hpp"

// --- [Begin] - Npcs ---
#include "Npc.hpp"
#include "QuestNpc.hpp"
#include "MerchantNpc.hpp"
// --- [End] ---

#include "EntityEffects.hpp"
// #include "NaturalEffects.hpp"
#include "ParticleManager.hpp"
#include "TextureManager.hpp"

#include "BossAltar.hpp"

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
    // Mythic	  = 0.0001  - 1 / 10000
    // Legendary  = 0.001   - 1 / 1000
    // Rare	      = 0.01    - 1 / 100
    // Unique     = 0.03    - 1 / 20
    // Normal     = 0.05    - 1 / 20

    std::vector<std::pair<float, std::shared_ptr<ItemData>>> batInventory;
    batInventory.emplace_back(0.05, std::make_shared<Bow>("Old Bow", "bow_00", 1.0f, 1, ItemRarity::Normal));
    batInventory.emplace_back(0.05, std::make_shared<Helmet>("Old Helmet", "helmet_00", 2.0f, 1, ItemRarity::Normal));
    batInventory.emplace_back(0.05, std::make_shared<Chestplate>("Old Chestplate", "chestplate_00", 2.0f, 1, ItemRarity::Normal));
    batInventory.emplace_back(0.05, std::make_shared<Leggings>("Old Leggings", "leggings_00", 2.0f, 1, ItemRarity::Normal));
    batInventory.emplace_back(0.05, std::make_shared<Boots>("Old Boots", "boots_00", 2.0f, 1, ItemRarity::Normal));
    batInventory.emplace_back(0.1, std::make_shared<Orb>("Bat Orb", "orb", ItemRarity::Rare));
    
    std::vector<std::pair<float, std::shared_ptr<ItemData>>> eyeInventory;
    eyeInventory.emplace_back(0.05, std::make_shared<Bow>("Wooden Bow", "bow_00", 2.5f, 3, ItemRarity::Unique));
    eyeInventory.emplace_back(0.05, std::make_shared<Helmet>("Copper Helmet", "helmet_00", 3.0f, 3, ItemRarity::Unique));
    eyeInventory.emplace_back(0.05, std::make_shared<Chestplate>("Copper Chestplate", "chestplate_00", 3.0f, 3, ItemRarity::Unique));
    eyeInventory.emplace_back(0.05, std::make_shared<Leggings>("Copper Leggings", "leggings_00", 3.0f, 3, ItemRarity::Unique));
    eyeInventory.emplace_back(0.05, std::make_shared<Boots>("Copper Boots", "boots_00", 3.0f, 3, ItemRarity::Unique));
    eyeInventory.emplace_back(0.1, std::make_shared<Orb>("Eye Orb", "orb", ItemRarity::Rare));

    for (const auto& pair : enemyRects) {
        for (const sf::FloatRect& rect : pair.second) {
            if (pair.first == "Bat Lv.1") {
                enemies.push_back(std::make_unique<Bat>(rect.getPosition(), batInventory));
            }
            else if (pair.first == "Bat Lv.3") {
                enemies.push_back(std::make_unique<Bat>(rect.getPosition(), batInventory));
            }
            else if (pair.first == "Eye Lv.5") {
                enemies.push_back(std::make_unique<Eye>(rect.getPosition(), eyeInventory));
            }
            else {
                std::cerr << "[Bug] - Main.cpp - loadEnemy()\n";
            }
        }
    }

    std::vector<std::pair<float, std::shared_ptr<ItemData>>> batBossInventory;
    batBossInventory.emplace_back(1.0f, std::make_shared<Bow>("Copper Bow", "bow_00", 5.0f, 5, ItemRarity::Legendary));
    batBossInventory.emplace_back(1.0f, std::make_shared<Helmet>("Copper Helmet", "helmet_00", 5.0f, 5, ItemRarity::Rare));
    batBossInventory.emplace_back(1.0f, std::make_shared<Chestplate>("Copper Chestplate", "chestplate_00", 5.0f, 5, ItemRarity::Rare));
    batBossInventory.emplace_back(1.0f, std::make_shared<Leggings>("Copper Leggings", "leggings_00", 5.0f, 5, ItemRarity::Rare));
    batBossInventory.emplace_back(1.0f, std::make_shared<Boots>("Copper Boots", "boots_00", 5.0f, 5, ItemRarity::Rare));

    // enemies.push_back(std::make_unique<BatBoss>(sf::Vector2f(176, 108) * 32.0f, batBossInventory));
}

void loadNpc(std::vector<std::unique_ptr<Npc>>& npcs, const TileMap& map) {
    npcs.push_back(std::make_unique<QuestNpc>(
        0,
        map.getQuestNpcRects().at(0),
        "Elder Thorne",
        "npc_00"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        1,
        map.getQuestNpcRects().at(1),
        "Torren",
        "npc_01"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        2,
        map.getQuestNpcRects().at(2),
        "Mira",
        "npc_02"
    ));
    npcs.push_back(std::make_unique<QuestNpc>(
        3,
        map.getQuestNpcRects().at(3),
        "Bren",
        "npc_03"
    ));

    npcs.push_back(std::make_unique<MerchantNpc>(
        map.getMerchantNpcRects().at(0),
        "Merchant",
        "npc_00"
    ));
}

void loadQuests(std::vector<Quest>& quests) {
    // --- [Begin] - Explore strange path --- 
    quests.push_back(Quest("Explore strange paths", 40));
    quests.back().addRequiredLevel(1);
    quests.back().addRequiredDescription("Find Elder Thorne. Min Lv.1");
    // Stage 0: Elder Thorne cảnh báo
    quests.back().addNpcID      (0, 0);
    quests.back().addDialogue   (0, "[1/4] Something stirs in the forest... It no longer feels safe");
    quests.back().addDialogue   (0, "[2/4] Long ago, we sealed away a dark presence beyond the hills");
    quests.back().addDialogue   (0, "[3/4] But now... the whispers return, and creatures creep closer each night");
    quests.back().addDialogue   (0, "[4/4] You must go, uncover the truth. Our hope rests with you");
    quests.back().addDescription(0, "Investigate the road beyond the wooden bridge");
    quests.back().addObjective  (0, std::make_shared<ExploreObjective>(1));
    // Stage 1: Quay về Elder Thorne
    quests.back().addNpcID      (1, 0);
    quests.back().addDialogue   (1, "[1/3] You're back... I feared the worst");
    quests.back().addDialogue   (1, "[2/3] This place was once peaceful. What have we awakened?");
    quests.back().addDialogue   (1, "[3/3] Find Torren. He'll help you prepare for what lies ahead");
    quests.back().addDescription(1, "Return to Elder Thorne");
    quests.back().addObjective  (1, std::make_shared<TalkObjective>(0));
    // Stage 2: Gặp Torren, nhận cung
    quests.back().addNpcID      (2, 1);
    quests.back().addDialogue   (2, "[1/7] Elder Thorne sent you? Then it must be serious");
    quests.back().addDialogue   (2, "[2/7] You're really heading out there? It's dangerous");
    quests.back().addDialogue   (2, "[3/7] I've seen strange tracks by the river. They weren't left by any animal");
    quests.back().addDialogue   (2, "[4/7] If you're going, take this bow. You'll need it");
    quests.back().addDialogue   (2, "[5/7] Press [E] to open your inventory, then drag the bow into your weapon slot");
    quests.back().addDialogue   (2, "[6/7] Press [Space] to use your bow. Stay sharp");
    quests.back().addDialogue   (2, "[7/7] From now on, you fight as an archer. Help us drive back the darkness");
    quests.back().addDescription(2, "Speak to Torren");
    quests.back().addObjective  (2, std::make_shared<TalkObjective>(1));
    quests.back().addItemFromNpc(2, std::make_shared<Bow>("Old Bow", "bow_00", 1.0f, 1, ItemRarity::Normal));
    // Stage 3: Diệt quái
    quests.back().addNpcID      (3, -1);
    quests.back().addDialogue   (3, std::string());
    quests.back().addDescription(3, "Help the villages defeat the monsters");
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Bat Lv.1", 5));
    quests.back().addObjective  (3, std::make_shared<KillMonsterObjective>("Eye Lv.5", 2));
    // Stage 4: Trở lại Torren
    quests.back().addNpcID      (4, 1);
    quests.back().addDialogue   (4, "[1/3] Back already? I see you've made it through");
    quests.back().addDialogue   (4, "[2/3] That bow served you well, huh? Told you it'd come in handy");
    quests.back().addDialogue   (4, "[3/3] You're stronger now. Maybe ready to carve your own legend");
    quests.back().addDescription(4, "Return to Torren");
    quests.back().addObjective  (4, std::make_shared<TalkObjective>(1));
    // --- [End] ---
    
    // --- [Begin] - Into the Darkwood ---
    quests.push_back(Quest("Into the Darkwood", 100));
    quests.back().addRequiredLevel(3);
    quests.back().addRequiredDescription("Find Mira. Min Lv.3");
    // Stage 0: Mira khởi đầu
    quests.back().addNpcID      (0, 2);
    quests.back().addDialogue   (0, "[1/4] I've heard strange noises deeper in the forest lately...");
    quests.back().addDialogue   (0, "[2/4] Something's not right. The animals are restless, the wind feels heavy");
    quests.back().addDialogue   (0, "[3/4] I think it's time you meet someone who knows more-Bren, the forest keeper");
    quests.back().addDialogue   (0, "[4/4] You'll find him beyond the old stone gate, deeper in the woods");
    quests.back().addDescription(0, "Head deeper into the forest and find Bren");
    quests.back().addObjective  (0, std::make_shared<TalkObjective>(2));
    // Stage 1: Gặp Bren
    quests.back().addNpcID      (1, 3);
    quests.back().addDialogue   (1, "[1/5] Mira sent you? Hmph... Then things must be worse than I thought");
    quests.back().addDialogue   (1, "[2/5] This forest holds secrets-old, dangerous ones");
    quests.back().addDialogue   (1, "[3/5] Before I trust you with more, bring me some samples from the creatures nearby");
    quests.back().addDialogue   (1, "[4/5] I need 3 Shadow Spores and 2 Black Leaves. You'll know them when you see them");
    quests.back().addDialogue   (1, "[5/5] Bring them here, and I'll tell you what really happened to Darkwood");
    quests.back().addDescription(1, "Head deeper into the forest and find Bren");
    quests.back().addObjective  (1, std::make_shared<TalkObjective>(3));
    // Stage 2: Kiếm đồ
    quests.back().addNpcID      (2, 3);
    quests.back().addDialogue   (2, std::string());
    quests.back().addDescription(2, "Collect materials for Bren");
    quests.back().addObjective  (2, std::make_shared<GiveItemObjective>("Wooden Bow", 2));
    quests.back().addObjective  (2, std::make_shared<GiveItemObjective>("Copper Helmet", 2));
    // Stage 3: Trả đồ, nghe kể chuyện
    quests.back().addNpcID      (3, 3);
    quests.back().addDialogue   (3, "[1/6] Ah, you've returned. Good. Let me see...");
    quests.back().addDialogue   (3, "[2/6] These spores... they reek of corruption");
    quests.back().addDialogue   (3, "[3/6] Decades ago, this forest was pure. But something came-something ancient");
    quests.back().addDialogue   (3, "[4/6] It twisted the trees, poisoned the roots, and consumed the heart of the woods");
    quests.back().addDialogue   (3, "[5/6] We tried to fight back, but the deeper we went, the darker it became");
    quests.back().addDialogue   (3, "[6/6] If you truly want to help, we must go further. Into the shadows");
    quests.back().addDescription(3, "Return to Bren with the items and listen to his tale");
    quests.back().addObjective  (3, std::make_shared<TalkObjective>(3));
    // --- [End] ---

    // quests.push_back(Quest(<name>, <exp>));
    // quests.back().addRequiredLevel(<playerLevel>);
    // quests.back().addRequiredDescription(<description>);

    // quests.back().addNpcID      (<stage>, <npcID>);
    // quests.back().addDialogue   (<stage>, <dialogue>);
    // quests.back().addDescription(<stage>, <description>);
    // quests.back().addObjective  (<stage>, <objective>);
}

void loadBossAltars(std::vector<BossAltar>& bossAltars, const std::unordered_map<int, sf::FloatRect>& bossAltarRects) {
    bossAltars.emplace_back(
        0,
        bossAltarRects.at(0)
    );
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Project_H", sf::Style::Close);
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    //  --- [Begin] - Loading ---
    Font::font.loadFromFile("Assets/Fonts/Roboto_Mono.ttf");
    TextureManager::loadSprite();
    EntityEffects::loadShader();

    // NaturalEffects naturalEffects;
    // naturalEffects.load("Assets/Shaders/naturalEffects.frag");
    // naturalEffects.loadSmartLightingShader("Assets/Shaders/smartLighting.frag");

    SoundManager::loadSound();

    TileMap map;
    map.loadMap();

    std::vector<std::unique_ptr<Enemy>> enemies;
    loadEnemy(enemies, map.getEnemyRects());

    std::vector<std::unique_ptr<Npc>> npcs;
    loadNpc(npcs, map);
    
    std::vector<Quest> quests;
    loadQuests(quests);

    sf::Vector2f PlayerTiles = sf::Vector2f( 78, 110);
    Player player(PlayerTiles * static_cast<float>(TILE_SIZE), 10.0f, std::move(quests));
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

    // int playerLightID = naturalEffects.addLight(player.getPosition(), 350.0f, sf::Vector3f(1.0f, 1.0f, 1.0f), 1.0f, 1);
    // for (const sf::Vector2f lightPosition : map.getLights()) {
    //     naturalEffects.addLight(lightPosition);
    // }
    
    std::vector<Item> items;
    items.emplace_back(player.getPosition() + sf::Vector2f(100.0f, 0), std::make_shared<Bow>("God Bow", "bow_00", 10.0f, 1, ItemRarity::Mythic));
    items.emplace_back(player.getPosition() + sf::Vector2f(300.0f, 0), std::make_shared<Helmet>("God Helmet", "helmet_00", 10000.0f, 1, ItemRarity::Mythic));

    InventoryUI inventoryUI(static_cast<sf::Vector2f>(window.getSize()), player);
    MerchantUI merchantUI(static_cast<sf::Vector2f>(window.getSize()), player);

    std::vector<BossAltar> bossAltars;
    loadBossAltars(bossAltars, map.getBossAltarRects());
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

        // Player collision Npcs
        for (const std::unique_ptr<Npc>& npc : npcs) {
            if (player.isCollision(npc->getHitbox())) {
                player.collisionWithNpc  = true;
                npc->collisionWithPlayer = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    npc->interactWithPlayer(player);
                }
            }
        }
        // Player collision Items
        for (auto it = items.begin(); it != items.end(); ) {
            if (it->canPickup() && player.isCollision(it->getHitbox())) {
                if (player.addItem(it->getItem())) {
                    it = items.erase(it); 
                }
                else {
                    ++it;
                }
            } else {
                ++it; 
            }
        }
        // Player collision Boss Altars
        for (BossAltar& bossAltar : bossAltars) {
            if (player.isCollision(bossAltar.getHitbox())) {
                bossAltar.collisionWithPlayer = true;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
                    bossAltar.interactWithPlayer(player, enemies);
                }
            }
        }

        player.update(dt, window, map.getCollisionRects(), map.getRegionRects());
        
        ui.update(dt, player, uiView.getSize(), gameClock);
        
        player.updateView(dt, view);
        
        for (auto& enemy : enemies) {
            enemy->update(dt, player, map.getCollisionRects(), items);
        }

        for (auto& npc : npcs) {
            npc->update(dt);
        }

        for (Item& item : items) {
            item.update(dt);
        }

        for (BossAltar& bossAltar : bossAltars) {
            bossAltar.update(dt);
        }
        
        map.update(dt);
        map.updateOverlayTransparency(player.getHitbox());
        for (const auto& enemy : enemies) {
            map.updateOverlayTransparency(enemy->getHitbox());
        }
        for (const Item& item : items) {
            map.updateOverlayTransparency(item.getHitbox());
        }
        
        particleManager.update(dt, view);
        particleManager.isCollisionWithCloud(player.getHitbox());
        for (const auto& enemy : enemies) {
            particleManager.isCollisionWithCloud(enemy->getHitbox());
        }
        for (const Item& item : items) {
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

        if (inventoryUI.isVisible()) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                inventoryUI.updateDrag(mousePos);
            }
            else {
                inventoryUI.updateHover(mousePos);    
            }
            inventoryUI.updateStats(player);
        }
        else {
            if (inventoryUI.isDrag()) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                inventoryUI.handleRelease(mousePos, player, items);
            }
        }

        if (merchantUI.isVisible()) {
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                merchantUI.updateDrag(mousePos);
            }
            else {
                merchantUI.updateHover(mousePos);    
            }
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

        for (auto& enemy : enemies) if (enemy->calculateDistance(player) <= LOADING_DISTANCE) {
            enemy->draw(sceneTexture);
        }

        for (auto& npc : npcs) {
            npc->draw(sceneTexture);
        }

        for (auto& item : items) {
            item.draw(sceneTexture);
        }

        player.draw(sceneTexture);
        
        map.drawOverlay(sceneTexture);

        sceneTexture.draw(particleManager);
        // --- [End] ---

        // --- [Begin] Draw layer 2 ---
        sceneTexture.setView(uiView);
        particleManager.drawScreen(sceneTexture);

        bool merchantFlag = false;
        for (std::unique_ptr<Npc>& npc : npcs) {
            sceneTexture.setView(view);
            npc->drawInteractText(sceneTexture);
            
            if (MerchantNpc* merchantNpc = dynamic_cast<MerchantNpc*>(npc.get())) {
                if (merchantNpc->isInteractWithPlayer()) {
                    merchantFlag = true;

                    if (inventoryUI.isVisible()) {
                        merchantNpc->interactWithPlayer(player);
                        merchantFlag = false;
                    }
                }   
            }
        }
        if (merchantUI.isVisible() && merchantFlag == false) {
            merchantUI.isPayment(player);
        }
        merchantUI.setVisible(merchantFlag);

        for (const BossAltar& bossAltar : bossAltars) {
            bossAltar.drawInteractText(sceneTexture);
        }
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