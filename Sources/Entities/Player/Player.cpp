#include "Player.hpp"

#include <cmath>

#include "ItemManager.hpp"
#include "ItemDatabase.hpp"
#include "QuestDatabase.hpp"

#include "Constants.hpp"
#include "Font.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "EntityEffects.hpp"
#include "Region.hpp"
#include "Normalize.hpp"
#include "AllItems.hpp"

Player::Player(const sf::Vector2f& position, const float& m_baseHp) 
: hitbox(position, sf::Vector2f(TILE_SIZE, TILE_SIZE)) {
    state           = 0;
    MOVE_SPEED      = 200.0f; 
    basePosition    = position;
    
    movingDirection = sf::Vector2f(0.f, 0.f);

    // --- [Begin] - Inventory ---
    equipmentHealth = 0.0f;
    equipmentDamage = 0.0f;
    
    for (int cnt = 0; cnt < 40; ++cnt) {
        inventory.push_back(nullptr);
    }
    
    for (int i = 0; i < 8; ++i) {
        equipment.push_back(nullptr);
    }
    // --- [End] - Inventory ---

    level            = 1;
    baseHp           = m_baseHp;
    baseHealthPoints = baseHp + 2.0f * (level - 1);
    healthPoints     = baseHealthPoints + equipmentHealth;
    damage           = equipmentDamage;
    BASE_EXPERIENCE  = 10.0f;
    xp               = 0.0f;

    DYING_TIME         = 1.0f;
    dyingCooldownTimer = 0.0f;
    
    RESPAWN_TIME         = 2.0f;
    respawnCooldownTimer = 0.0f;

    INVINCIBLE_TIME         = 5.0f;
    invincibleCooldownTimer = INVINCIBLE_TIME;

    VIEW_LEAP_SPEED = 2.0f;

    dashDirection     = sf::Vector2f(0.f, 0.f); 
    isDashing         = false;                    
    DASH_SPEED        = MOVE_SPEED * 2;          
    DASH_DURATION     = 0.4f;                     
    dashTimer         = 0.0f;                    
    DASH_COOLDOWN     = DASH_DURATION + 0.2f;     
    dashCooldownTimer = 0.0f;                     

    loadingBox.setRadius(LOADING_DISTANCE);
    loadingBox.setOutlineColor(sf::Color::Blue);
    loadingBox.setOutlineThickness(1.f);
    loadingBox.setFillColor(sf::Color::Transparent);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - LOADING_DISTANCE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - LOADING_DISTANCE
    );

    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_UP_LEFT) , TextureManager::get("playerSprite"), 19, 21, 2,  8, 0.5f , true);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_UP_RIGHT), TextureManager::get("playerSprite"), 19, 21, 2,  8, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::IDLE_DOWN)    , TextureManager::get("playerSprite"), 19, 21, 2,  7, 0.5f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 4,  0, 0.1f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 4,  0, 0.1f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_UP_LEFT) , TextureManager::get("playerSprite"), 19, 21, 3,  1, 0.1f , true );
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_UP_RIGHT), TextureManager::get("playerSprite"), 19, 21, 3,  1, 0.1f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::WALK_DOWN)    , TextureManager::get("playerSprite"), 19, 21, 3,  0, 0.1f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DASH_LEFT)    , TextureManager::get("playerSprite"), 19, 21, 4,  3, 0.09f, true );
    animationManager.addAnimation(static_cast<int>(PlayerState::DASH_RIGHT)   , TextureManager::get("playerSprite"), 19, 21, 4,  3, 0.09f, false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DYING)        , TextureManager::get("playerSprite"), 19, 21, 4,  9, 0.2f , false);
    animationManager.addAnimation(static_cast<int>(PlayerState::DEAD)         , TextureManager::get("playerSprite"), 19, 21, 4, 10, 0.5f , false);

    shadow = Animation(TextureManager::get("playerShadow"), 13, 5, 1, 0, 0.f, false);

    projectiles         = std::vector<Projectile>();
    SHOOT_COOLDOWN      = 0.2f;
    PROJECTILE_SPEED    = MOVE_SPEED * 2;
    PROJECTILE_LIFETIME = 1.0f;
    shootCooldownTimer  = 0.0f;

    KNOCKBACK_STRENGTH     = 100.0f;
    KNOCKBACK_COOLDOWN     = 0.2f;
    knockbackCooldownTimer = 0.0f;

    // --- [Begin] --- Quests ---
    int questsSize = QuestDatabase::getQuestsSize();
    for (int questID = 0; questID < questsSize; ++questID) {
        quests.push_back(QuestProgress(questID));
    }
    INTERUPTED_TIME         = 0.5f;
    interuptedCooldownTimer = -11.0f;
    // --- [End] ---

    collisionRegionID = -1;
}

void Player::handleMove(const sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movingDirection.x = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movingDirection.x =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movingDirection.y =  1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movingDirection.y = -1.f;
    }

    movingDirection = Normalize::normalize(movingDirection);
}

void Player::handleDash(const sf::RenderWindow& window) {
    if (!isDashing && movingDirection != sf::Vector2f(0, 0)) {
        dashDirection = sf::Vector2f(0, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && dashCooldownTimer <= 0.f && !isDashing) {
        isDashing         = true;
        dashTimer         = DASH_DURATION;      // Đặt thời gian lướt
        dashCooldownTimer = DASH_COOLDOWN; // Đặt thời gian hồi chiêu

        // Xác định hướng lướt: dựa trên hướng di chuyển hiện tại.
        // Nếu đứng yên, lướt theo hướng đang nhìn.
        if (dashDirection.x != 0 || dashDirection.y != 0 ) {
            dashDirection = dashDirection;
        }
        else if (movingDirection.x != 0 || movingDirection.y != 0) {
            dashDirection = movingDirection;
        } 
        else {
            switch (state) {
                // case static_cast<int>(PlayerState::DASH_LEFT):
                case static_cast<int>(PlayerState::IDLE_LEFT):
                case static_cast<int>(PlayerState::WALK_LEFT):
                    dashDirection = sf::Vector2f(-1.0f,  0.0f); 
                    break;
                // case static_cast<int>(PlayerState::DASH_RIGHT):
                case static_cast<int>(PlayerState::IDLE_RIGHT):
                case static_cast<int>(PlayerState::WALK_RIGHT):
                    dashDirection = sf::Vector2f( 1.0f,  0.0f); 
                    break;
                case static_cast<int>(PlayerState::IDLE_UP_LEFT):
                case static_cast<int>(PlayerState::WALK_UP_LEFT): 
                case static_cast<int>(PlayerState::IDLE_UP_RIGHT):
                case static_cast<int>(PlayerState::WALK_UP_RIGHT): 
                    dashDirection = sf::Vector2f( 0.0f, -1.0f);
                    break;
                case static_cast<int>(PlayerState::IDLE_DOWN):
                case static_cast<int>(PlayerState::WALK_DOWN):
                    dashDirection = sf::Vector2f( 0.0f,  1.0f);
                    break;
                default:
                    std::cerr << "[Bug] - Player::handleInput() set dashDirection\n";
                    break;
            }
        }

        SoundManager::playSound("roll");
    }
}

void Player::handleProjectiles(const sf::RenderWindow& window) {
    if (damage <= 0.0f) {
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldownTimer <= 0.f && !isDashing) {
        shootCooldownTimer = SHOOT_COOLDOWN; 
        
        sf::Vector2f projectileDirection;
        if (movingDirection.x != 0 || movingDirection.y != 0) {
            projectileDirection = movingDirection;
        }
        else {
            switch (state) {
                case static_cast<int>(PlayerState::DASH_LEFT):
                case static_cast<int>(PlayerState::IDLE_LEFT):
                case static_cast<int>(PlayerState::WALK_LEFT):
                    projectileDirection = sf::Vector2f(-1.0f,  0.0f); 
                    break;
                case static_cast<int>(PlayerState::DASH_RIGHT):
                case static_cast<int>(PlayerState::IDLE_RIGHT):
                case static_cast<int>(PlayerState::WALK_RIGHT):
                    projectileDirection = sf::Vector2f( 1.0f,  0.0f); 
                    break;
                case static_cast<int>(PlayerState::IDLE_UP_LEFT):
                case static_cast<int>(PlayerState::WALK_UP_LEFT): 
                case static_cast<int>(PlayerState::IDLE_UP_RIGHT):
                case static_cast<int>(PlayerState::WALK_UP_RIGHT): 
                    projectileDirection = sf::Vector2f( 0.0f, -1.0f);
                    break;
                case static_cast<int>(PlayerState::IDLE_DOWN):
                case static_cast<int>(PlayerState::WALK_DOWN):
                    projectileDirection = sf::Vector2f( 0.0f,  1.0f);
                    break;
                default:
                    std::cerr << "[Bug] - Player::handleInput() set projectileDirection\n";
                    break;
            }
        }

        projectiles.emplace_back(
            TextureManager::get("arrow"),
            getCenterPosition(),
            projectileDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );

        SoundManager::playSound("arrow");
    }
}

void Player::handleInput(const float& dt, const sf::RenderWindow& window) {
    if (knockbackCooldownTimer <= 0) {
        movingDirection = sf::Vector2f(0, 0);
    }
    else {
        dashDirection = sf::Vector2f(0, 0);
        // fix here
        return;
    }

    handleMove(window);
    
    handleDash(window);

    handleProjectiles(window);
}

bool Player::isCollisionProjectiles(const sf::FloatRect& rect) {
    for (Projectile& projectile : projectiles) {
        if (projectile.isAlive() && projectile.isCollision(rect)) {
            projectile.kill();
            return true;
        }
    }

    return false;
}

bool Player::isCollision(const sf::FloatRect& rect) const {
    return hitbox.intersects(rect);
}

bool Player::isAlive() const {
    return state != static_cast<int>(PlayerState::DYING) && state != static_cast<int>(PlayerState::DEAD);
}

void Player::hurt(const float& damage) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        healthPoints -= damage;

        SoundManager::playSound("playerHurt");

        if (healthPoints <= 0) {
            healthPoints = 0.0f;
            kill();
        }
    }
}

void Player::knockback(const sf::Vector2f& enemyPosition) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        movingDirection = (hitbox.getPosition() - enemyPosition);

        movingDirection = Normalize::normalize(movingDirection);

        knockbackCooldownTimer = KNOCKBACK_COOLDOWN;
    }
}

void Player::kill() {
    healthPoints       = 0;

    state              = static_cast<int>(PlayerState::DYING);
    dyingCooldownTimer = DYING_TIME;
    
    SoundManager::playSound("playerDie");

    projectiles.clear();
}

void Player::respawn() {
    if (state == static_cast<int>(PlayerState::DEAD) && respawnCooldownTimer <= 0) {
        hitbox.left = basePosition.x;
        hitbox.top  = basePosition.y;
        
        movingDirection = sf::Vector2f(0.f, 0.f);

        loadingBox.setPosition(getCenterPosition() - sf::Vector2f(LOADING_DISTANCE, LOADING_DISTANCE));
        
        state = 0;

        healthPoints = baseHealthPoints + equipmentHealth;

        invincibleCooldownTimer = INVINCIBLE_TIME;
    }
}

sf::FloatRect Player::getHitbox() const {
    return hitbox;
}

void Player::updateTimer(const float &dt) {
    if (dyingCooldownTimer > 0) {
        dyingCooldownTimer -= dt;
    }
    else {
        if (state == static_cast<int>(PlayerState::DYING)) {            
            state                = static_cast<int>(PlayerState::DEAD); 
            respawnCooldownTimer = RESPAWN_TIME;
            
            return;
        }
    }
    if (respawnCooldownTimer > 0) {
        respawnCooldownTimer -= dt;
    }
    if (invincibleCooldownTimer > 0) {
        invincibleCooldownTimer -= dt;
    }

    if (dashTimer > 0) {
        dashTimer -= dt;
    }
    else {
        isDashing = false;
    }
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    if (shootCooldownTimer > 0) { 
        shootCooldownTimer -= dt;
    }
    if (knockbackCooldownTimer > 0) {
        knockbackCooldownTimer -= dt;
    }
    if (interuptedCooldownTimer > 0) {
        interuptedCooldownTimer -= dt;
    }
}

void Player::updateCollisionArea(const float& dt, const std::unordered_map<int, sf::FloatRect>& regionRects) {
    if (collisionWithNpc) {
        collisionWithNpc        = false;
        interuptedCooldownTimer = INTERUPTED_TIME;
    }
    else if (interuptedCooldownTimer != -11.0f) {
        if (interuptedCooldownTimer <= 0) {
            for (QuestProgress& quest : quests) {
                quest.isInterruptedGivingQuest();
            }
            interuptedCooldownTimer = -11.0f;
        }
    }

    for (auto& regionRect : regionRects) {
        if (isCollision(regionRect.second)) {
            for (QuestProgress& quest : quests) {
                QuestEventData dataPack;
                dataPack.eventType = "explore";
                dataPack.regionID  = regionRect.first;
                quest.update(dataPack);
            }

            collisionRegionID = regionRect.first;

            // SoundManager::resumeMusic(dt, "region" + std::to_string(regionRect.first));
        }
    }
}

void Player::updatePosition(const float& dt, const std::vector<sf::FloatRect>& collisionRects) {
    sf::Vector2f velocity(0.f, 0.f);
    if (knockbackCooldownTimer > 0) {
        velocity = movingDirection * KNOCKBACK_STRENGTH * dt;
    }
    else if (isDashing) {
        velocity = dashDirection   * DASH_SPEED * dt;
    } else {
        velocity = movingDirection * MOVE_SPEED * dt;
    }

    sf::FloatRect nextHitbox = hitbox;
    if (velocity.x != 0) {
        nextHitbox.left += velocity.x;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitbox)) { 
                if (std::abs(velocity.x) > COLLISION_EPSILON) {
                    nextHitbox.left -= velocity.x / 10.0f;
                }
                else {
                    nextHitbox.left -= velocity.x;
                    // std::cerr << "[Bug] - Player.cpp - updatePosition()\n";
                }
            }
        }
    }
    if (velocity.y != 0) {
        nextHitbox.top += velocity.y;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitbox)) {
                if (std::abs(velocity.y) > COLLISION_EPSILON) {
                    nextHitbox.top -= velocity.y / 10.0f;
                }
                else {
                    nextHitbox.top -= velocity.y;
                    // std::cerr << "[Bug] - Player.cpp - updatePosition()\n";
                }
            }
        }
    }

    hitbox = nextHitbox;
}

void Player::updateHitbox() {
    loadingBox.setPosition(getCenterPosition() - sf::Vector2f(LOADING_DISTANCE, LOADING_DISTANCE));
}

void Player::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (isDashing) {
        state = (dashDirection.x < 0 ? static_cast<int>(PlayerState::DASH_LEFT) : static_cast<int>(PlayerState::DASH_RIGHT));
    }
    else if (dashDirection != sf::Vector2f(0, 0)) {
        if (dashDirection.y < 0) {
            state = (dashDirection.x < 0 ? static_cast<int>(PlayerState::IDLE_UP_LEFT) : static_cast<int>(PlayerState::IDLE_UP_RIGHT));
        }
        else if (dashDirection.x != 0) {
            state = (dashDirection.x < 0 ? static_cast<int>(PlayerState::IDLE_LEFT) : static_cast<int>(PlayerState::IDLE_RIGHT));
        }
        else if (dashDirection.y > 0) {
            state = static_cast<int>(PlayerState::IDLE_DOWN);
        }
    }
    else if (movingDirection == sf::Vector2f(0, 0)) {
        if (state == static_cast<int>(PlayerState::WALK_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_RIGHT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_UP_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_UP_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_UP_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_UP_RIGHT);
        }
        else if (state == static_cast<int>(PlayerState::WALK_DOWN)) {
            state = static_cast<int>(PlayerState::IDLE_DOWN);
        }
        else if (state == static_cast<int>(PlayerState::DASH_LEFT)) {
            state = static_cast<int>(PlayerState::IDLE_LEFT);
        }
        else if (state == static_cast<int>(PlayerState::DASH_RIGHT)) {
            state = static_cast<int>(PlayerState::IDLE_RIGHT);
        }
    }
    else if (movingDirection.y < 0) {
        state = (movingDirection.x < 0 ? static_cast<int>(PlayerState::WALK_UP_LEFT) : static_cast<int>(PlayerState::WALK_UP_RIGHT));
    }
    else if (movingDirection.x != 0) {
        state = (movingDirection.x < 0 ? static_cast<int>(PlayerState::WALK_LEFT) : static_cast<int>(PlayerState::WALK_RIGHT));
    }
    else if (movingDirection.y > 0) {
        state = static_cast<int>(PlayerState::WALK_DOWN);
    }

    animationManager.setState(state);
    animationManager.setPosition(hitbox.getPosition() - sf::Vector2f(3, 6));
    animationManager.update();

    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(3, hitbox.getSize().y - 5));
}

void Player::updateProjectiles(const float& dt) {
    for (auto& p : projectiles) {
        p.update(dt);
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); /**/) {
        if (!it->isAlive()) {
            it = projectiles.erase(it);
        } 
        else {
            ++it;
        }
    }
}

void Player::updateQuests() {
    int lastFinishedQuestID = -1;
    for (QuestProgress& quest : quests) {
        if (quest.isCompleted()) {
            lastFinishedQuestID = std::max(lastFinishedQuestID, quest.getQuestID());
        }
    }

    for (auto it = quests.begin(); it != quests.end(); /**/) {
        if (it->isLocked()) {
            it->unlock(lastFinishedQuestID);
        }

        if (it->shouldGiveItemForPlayer()) {
            std::vector<std::shared_ptr<ItemData>> npcItems = it->getNpcItem();
            for (auto& item : npcItems) {
                addItem(item);
            }
        }

        if (it->updateStage()) {
            updateQuest = true;
        }
        
        if (it->isCompleted() && it->isRewardReceived() == false) {
            updateXP(it->getRewardExp());
        }

        ++it;
    }
}

void Player::update(const float& dt, 
                    const sf::RenderWindow& window, 
                    const std::vector<sf::FloatRect>& collisionRects, 
                    const std::unordered_map<int, sf::FloatRect>& regionRects) {

    updateTimer(dt);

    if (!isAlive()) {
        updateHitbox();

        updateAnimation();
        
        respawn();

        return;
    }
    
    updateCollisionArea(dt, regionRects);

    handleInput(dt, window);

    updatePosition(dt, collisionRects);

    updateHitbox();

    updateAnimation();

    updateProjectiles(dt); 

    updateQuests();
}

void Player::draw(sf::RenderTarget& target) {
    // sf::RectangleShape hitboxShape;
    // hitboxShape.setPosition(hitbox.getPosition());
    // hitboxShape.setSize(hitbox.getSize());
    // hitboxShape.setOutlineColor(sf::Color::Red);
    // hitboxShape.setOutlineThickness(1.f);
    // hitboxShape.setFillColor(sf::Color::Transparent);
    // target.draw(hitboxShape);
    // target.draw(loadingBox);

    shadow.draw(target);

    if (invincibleCooldownTimer > 0) {
        animationManager.draw(target, EntityEffects::get("invincible"));
    }
    else if (knockbackCooldownTimer > 0) {
        animationManager.draw(target, EntityEffects::get("flash"));   
    }
    else {
        animationManager.draw(target);
    }

    for (auto& p : projectiles) {
        p.draw(target);
    }
}

sf::Vector2f Player::getPosition() const {
    return hitbox.getPosition();
}

void Player::levelUp() {
    level++;

    baseHealthPoints += 2.0f;
    healthPoints = baseHealthPoints + equipmentHealth;

    SoundManager::playSound("levelUp");
}

float Player::XPRequired() const {
    return BASE_EXPERIENCE * (level / 10 + 1) * level * level;
}

void Player::updateLevel() {
    while (xp >= XPRequired()) {
        xp -= XPRequired();
        
        levelUp();
    }
}


void Player::updateXP(const float& amount) {
    xp += amount;

    updateLevel();
}

void Player::addVictim(const std::string& label, const float& expAmount) {
    for (QuestProgress& quest : quests) {
        QuestEventData dataPack;
        dataPack.eventType  = "kill";
        dataPack.targetName = label;
        quest.update(dataPack);
    }

    updateXP(expAmount);
}

float Player::getHealthRatio() const {
    if (baseHealthPoints + equipmentHealth == 0) {
        return 1.0f;
    }
    return healthPoints / (baseHealthPoints + equipmentHealth);
}

std::string format2Decimals(float value) {
    std::string str = std::to_string(value);
    return str.substr(0, str.find('.') + 3);
}

std::string Player::getHealthPointsString() const {
    return format2Decimals(healthPoints) + '/' + format2Decimals(baseHealthPoints + equipmentHealth);
}

float Player::getXPRatio() const {
    return xp / XPRequired();
}

std::string Player::getXPString() const {
    return format2Decimals(xp) + '/' + format2Decimals(XPRequired());
}

int Player::getLevel() const {
    return level;
}

std::vector<QuestProgress>& Player::getQuests() {
    return quests;
}

bool Player::isUpdateQuest() {
    if (updateQuest) {
        updateQuest = false;
    
        return true;
    }

    return false;
}

sf::Vector2f Player::getCenterPosition() const {
    return hitbox.getPosition() + hitbox.getSize() / 2.0f;
}

void Player::updateView(const float& dt, sf::View& view) const {
    sf::Vector2f currentCenter = view.getCenter();
    sf::Vector2f targetCenter  = getCenterPosition();
    sf::Vector2f lerped        = currentCenter + VIEW_LEAP_SPEED * (targetCenter - currentCenter) * dt;
    lerped.x = std::round(lerped.x);
    lerped.y = std::round(lerped.y);
    view.setCenter(lerped);
}

int Player::getCollisionRegionID() const {
    return collisionRegionID;
}

// --- [Begin] - Inventory --- 
bool Player::addItem(const std::shared_ptr<ItemData>& newItem) {
    bool added = false;
    for (auto& item : inventory) if (item) {
        if (item->name == newItem->name) {
            item->amount += newItem->amount;
            
            SoundManager::playSound("pickupItem");

            added = true;

            break;
        }
    }

    if (!added) {
        for (auto& item : inventory) {
            if (!item) {
                item = newItem;

                SoundManager::playSound("pickupItem");

                added = true;

                break;
            }
        }
    }

    if (added) {
        for (QuestProgress& quest : getQuests()) if (!quest.isCompleted()) {
            for (auto& objective : quest.getQuestObjectives()) if (!objective->isFinished()) { 
                QuestEventData objectiveData = objective->getQuestEventData();
                if (objectiveData.eventType == "collectItem") {
                    if (newItem->name == objectiveData.targetName) {
                        QuestEventData collectItemData;
                        collectItemData.eventType  = "collectItem";
                        collectItemData.targetName = newItem->name;
                        collectItemData.amount     = newItem->amount;
                        quest.update(collectItemData);
                        
                        if (objective->isFinished()) {
                            break;
                        }
                    }
                }
            }
        }

        return true;
    } 

    return false; // Hết chỗ
}

void Player::updateEquipmentStats() {
    equipmentHealth = 0.0f;
    equipmentDamage = 0.0f;
    for (const auto& item : equipment) if (item) {
        if (auto equipItem = dynamic_cast<EquipItem*>(item.get())) {
            equipmentHealth += equipItem->getHealth();
            equipmentDamage += equipItem->getDamage();
        }
    }

    if (healthPoints > baseHealthPoints + equipmentHealth) {
        healthPoints = baseHealthPoints + equipmentHealth;
    }
    damage = equipmentDamage;
}

std::vector<std::shared_ptr<ItemData>>* Player::getInventory() {
    return &inventory;
}

std::vector<std::shared_ptr<ItemData>>* Player::getEquipment() {
    return &equipment;
}

std::string Player::getStats() const {
    std::string healthString          = "HP      : " + getHealthPointsString();
    std::string baseHealthString      = "Base HP : " + std::to_string(baseHealthPoints);
    std::string equipmentHealthString = "Equip HP: " + std::to_string(equipmentHealth);
    std::string damageString          = "Damage  : " + std::to_string(equipmentDamage);
    std::string xpString              = "EXP     : " + getXPString();
    std::string goldsString           = "Golds   : " + std::to_string(golds);

    baseHealthString      = baseHealthString.substr(0, baseHealthString.find('.') + 3);
    equipmentHealthString = equipmentHealthString.substr(0, equipmentHealthString.find('.') + 3);
    damageString          = damageString.substr(0, damageString.find('.') + 3);
    goldsString           = goldsString.substr(0, goldsString.find('.') + 3);
    
    return healthString + "\n"
         + baseHealthString + "\n" 
         + equipmentHealthString + "\n" 
         + damageString + "\n\n"
         + xpString + "\n"
         + goldsString;
}

bool Player::dropItem(const std::shared_ptr<ItemData>& item, ItemManager& items) {
    items.addItem(getCenterPosition(), item);
    updateEquipmentStats();

    SoundManager::playSound("pickupItem");

    return true;
}
// --- [End] - Inventory --- 

const float& Player::getDamage() const {
    return damage;
};

float Player::getXp() const {
    return xp;
}

std::vector<std::pair<std::string, int>> Player::getInventoryString() const {
    std::vector<std::pair<std::string, int>> inventoryString;
    for (const std::shared_ptr<ItemData>& item : inventory) {
        if (item == nullptr) {
            inventoryString.emplace_back(std::string(), 0);
        }
        else {
            inventoryString.emplace_back(item->name, item->amount);
        }
    }
    return inventoryString;
}

std::vector<std::pair<std::string, int>> Player::getEquipmentString() const {
    std::vector<std::pair<std::string, int>> equipmentString;
    for (const std::shared_ptr<ItemData>& item : equipment) {
        if (item == nullptr) {
            equipmentString.emplace_back(std::string(), 0);
        }
        else {
            equipmentString.emplace_back(item->name, item->amount);
        }
    }
    return equipmentString;
}

std::vector<QuestProgressData> Player::getQuestsData() const {
    std::vector<QuestProgressData> questsString;
    for (const QuestProgress& quest : quests) {
        questsString.push_back(quest.getData());
    }
    return questsString;
}

void Player::setPosition(const sf::Vector2f& m_position) {
    hitbox.left = m_position.x, hitbox.top = m_position.y;
}

void Player::setLevel(const int& m_level) {
    level = m_level;
}

void Player::setXp(const float& m_xp) {
    xp = m_xp;
}

void Player::setInventory(const std::vector<std::pair<std::string, int>>& inventoryString) {
    int inventorySize = inventory.size();
    for (int index = 0; index < inventorySize; ++index) {
        inventory[index] = ItemDatabase::get(inventoryString[index].first, inventoryString[index].second);
    }
}

void Player::setEquipment(const std::vector<std::pair<std::string, int>>& equipmentString) {
    int equipmentSize = equipment.size();
    for (int index = 0; index < equipmentSize; ++index) {
        equipment[index] = ItemDatabase::get(equipmentString[index].first, equipmentString[index].second);
    }
    updateEquipmentStats();
}

void Player::setQuests(const std::vector<QuestProgressData>& questsData) {
    int questsSize = quests.size();
    for (int index = 0; index < questsSize; ++index) {
        quests[index].setData(questsData[index]);
    }
}

void Player::modifierAfterLoad() {
    invincibleCooldownTimer = INVINCIBLE_TIME;
    
    baseHealthPoints = baseHp + 2.0f * (level - 1);
    if (healthPoints > baseHealthPoints + equipmentHealth) {
        healthPoints = baseHealthPoints + equipmentHealth;
    }
}