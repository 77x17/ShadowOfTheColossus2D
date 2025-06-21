#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Constants.hpp"
#include "AnimationManager.hpp"
#include "Projectile.hpp"
#include "SoundManager.hpp"
#include "Font.hpp"

#include "Quest.hpp"

#include "Item.hpp"

#include "InventorySlot.hpp"

enum class PlayerState {
    DYING = -2,
    DEAD  = -1,
    IDLE_LEFT,
    IDLE_RIGHT,
    IDLE_UP_LEFT,
    IDLE_UP_RIGHT,
    IDLE_DOWN,
    WALK_LEFT,
    WALK_RIGHT,
    WALK_UP_LEFT,
    WALK_UP_RIGHT,
    WALK_DOWN,
    DASH_LEFT,
    DASH_RIGHT
};

class Player {
private:
    // const
    sf::FloatRect hitbox;

    int           state;
    float         MOVE_SPEED;
    sf::Vector2f  basePosition;
    sf::Vector2f  movingDirection;

    float DYING_TIME;
    float dyingCooldownTimer;
    
    float RESPAWN_TIME;
    float respawnCooldownTimer;
    
    float baseHealthPoints;
    float healthPoints;
    float damage;
    float BASE_EXPERIENCE;
    int   level;
    float xp;

    float INVINCIBLE_TIME;
    float invincibleCooldownTimer;

    float VIEW_LEAP_SPEED;

    sf::Vector2f dashDirection;         // Hướng lướt
    bool         isDashing;             // Có đang lướt không?
    float        DASH_SPEED;            // Tốc độ khi lướt
    float        DASH_DURATION;         // Lướt trong bao nhiêu giây
    float        dashTimer;             // Đếm thời gian lướt
    float        DASH_COOLDOWN;         // Hồi chiêu bao nhiêu giây
    float        dashCooldownTimer;     // Đếm thời gian hồi chiêu

    sf::CircleShape    loadingBox;
    AnimationManager   animationManager;
    Animation          shadow;

    std::vector<Projectile> projectiles;
    float                   SHOOT_COOLDOWN;
    float                   PROJECTILE_SPEED;
    float                   PROJECTILE_LIFETIME;
    float                   shootCooldownTimer;

    float KNOCKBACK_STRENGTH;
    float KNOCKBACK_COOLDOWN;
    float knockbackCooldownTimer;

    std::vector<Quest> quests;
    float INTERUPTED_TIME;
    float interuptedCooldownTimer;
    
    int collisionRegionID;

    // --- [Begin] - Inventory --- 
    float equipmentHealth;
    float equipmentDamage;

    std::vector<std::shared_ptr<ItemData>> inventory;   // 10 x 4 = 40
    std::vector<std::shared_ptr<ItemData>> equipment;   // 8
    // --- [End] - Inventory --- 
public:
    bool collisionWithNpc = false;
    bool updateQuest      = false;

public:
    Player(const sf::Vector2f& position, const float& baseHp, std::vector<Quest>&& _quests);

    void handleMove(const sf::RenderWindow& window);
    void handleDash(const sf::RenderWindow& window);
    void handleProjectiles(const sf::RenderWindow& window);
    void handleInput(const float& dt, const sf::RenderWindow& window);

    bool isCollisionProjectiles(const sf::FloatRect& rect);
    bool isCollision(const sf::FloatRect& rect) const;

    bool isAlive() const;
    void hurt(const float& damage);
    void knockback(const sf::Vector2f& enemyPosition);
    void kill();
    void respawn();

    sf::FloatRect getHitbox() const;

    void updateTimer(const float &dt);
    void updateCollisionArea(const float& dt, const std::unordered_map<int, sf::FloatRect>& regionRects);
    void updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects);
    void updateHitbox();
    void updateAnimation();
    void updateProjectiles(const float& dt);
    void updateQuests();
    void update(const float& dt, 
                const sf::RenderWindow& window, 
                const std::vector<sf::FloatRect>& collisionRects, 
                const std::unordered_map<int, sf::FloatRect>& regionRects);

    void draw(sf::RenderTarget& target);

    sf::Vector2f getPosition() const;

    void  levelUp();
    void  updateLevel();
    float XPRequired() const;
    void  updateXP(const float& amount);
    void  addVictim(const std::string& label);

    // --- [Begin] - UI ---
    float               getHealthRatio() const;
    std::string         getHealthPointsString() const;
    float               getXPRatio() const;
    std::string         getXPString() const;
    int                 getLevel() const;
    std::vector<Quest>& getQuests();
    bool                isUpdateQuest();
    sf::Vector2f        getCenterPosition() const;
    int                 getCollisionRegionID() const;
    // --- [End]

    void updateView(const float& dt, sf::View& view) const;

    // --- [Begin] - Inventory --- 
    bool addItem(const std::shared_ptr<ItemData>& newItem);
    void updateEquipmentStats();
    std::vector<std::shared_ptr<ItemData>>* getInventory();
    std::vector<std::shared_ptr<ItemData>>* getEquipment();
    std::string getStats() const;
    bool dropItem(const std::shared_ptr<ItemData>& item, std::vector<Item>& items);
    // --- [End]

    // --- [Begin] - Enemy --- 
    const float& getDamage() const;
    // --- [End]
};