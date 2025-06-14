#include "Player.hpp"
#include "ShaderManager.hpp"

#include <cmath>

Player::Player(const float& x, const float& y, const float& hp, std::vector<Quest>&& _quests) {
    state           = 0;
    MOVE_SPEED      = 200.0f; 
    size            = sf::Vector2f(TILE_SIZE, TILE_SIZE);
    basePosition    = sf::Vector2f(x, y);
    position        = basePosition;
    movingDirection = sf::Vector2f(0.f, 0.f);

    maxHealthPoints = hp;
    healthPoints    = hp;
    BASE_EXPERIENCE = 10.0f;
    level           = 1;
    xp              = 0.0;

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

    hitbox.setSize(size);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.f);
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setPosition(x, y);

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

    FADE_SPEED            = 5.0f;
    interactTextOpacity   = 0.0f;
    INTERACT_COOLDOWN     = 0.5f;
    interactCooldownTimer = 0.0f;

    interactText.setFont(Font::font);
    interactText.setCharacterSize(12.5f);
    interactText.setOutlineThickness(2.0f);
    interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
    interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
    interactText.setString("Press [F] to talk");
    interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                           interactText.getLocalBounds().top + interactText.getLocalBounds().height / 2);

    KNOCKBACK_STRENGTH     = 100.0f;
    KNOCKBACK_COOLDOWN     = 0.2f;
    knockbackCooldownTimer = 0.0f;

    quests.clear();
    quests        = std::move(_quests);
    updateQuest   = false;
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

    movingDirection = Projectile::normalize(movingDirection);
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
            position + size / 2.f,
            projectileDirection,
            PROJECTILE_SPEED,
            PROJECTILE_LIFETIME
        );

        SoundManager::playSound("arrow");
    }
}

void Player::handleQuests(const float& dt, const sf::RenderWindow& window, std::vector<Npc>& npcs) {
    bool isCollisionNpc = false;
    for (const Npc& npc : npcs) {
        sf::FloatRect npcRect = npc.getHitbox();
        if (isCollision(npcRect)) {
            interactText.setPosition(npcRect.getPosition() + sf::Vector2f(npcRect.getSize().x / 2, -npcRect.getSize().y));
            isCollisionNpc = true;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && interactCooldownTimer <= 0) {
        for (Npc& npc : npcs) {
            if (isCollision(npc.getHitbox())) {
                for (Quest& quest : quests) {
                    QuestEventData dataPack;
                    dataPack.eventType   = "talk";
                    dataPack.targetNpcID = npc.getID();
                    quest.update(dataPack);

                    if (npc.getID() == quest.getID()) {
                        if (quest.isSuitableForGivingQuest(getLevel())) {
                            if (quest.isCompleted()) {
                                interactText.setString("Thanks for your help!");
                                continue;
                            }
                            else if (quest.isFinishedDialogue()) {
                                if (quest.accept()) {
                                    updateQuest = true;
                                    
                                    quest.update(QuestEventData());
                                }
                                else {
                                    interactText.setString("I need your support");

                                    continue;
                                }
                            }
                            else {
                                interactText.setString(quest.getDialogue());
                            }
                        }
                        else {
                            interactText.setString(quest.getRequired());
                        }

                        break;
                    }
                }

                interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                                       interactText.getLocalBounds().top + interactText.getLocalBounds().height / 2);
            }
        }
        interactCooldownTimer = INTERACT_COOLDOWN;
    }

    if (isCollisionNpc) {
        interactTextOpacity += (255 - interactTextOpacity) * FADE_SPEED * dt;
    }
    else {
        interactTextOpacity += (0   - interactTextOpacity) * FADE_SPEED * dt;

        if (40 < interactTextOpacity && interactTextOpacity < 50) {
            interactText.setString("Press [F] to talk");
            interactText.setOrigin(interactText.getLocalBounds().left + interactText.getLocalBounds().width / 2, 
                                   interactText.getLocalBounds().top + interactText.getLocalBounds().height / 2);

            for (Quest& quest : quests) {
                quest.isInterruptedGivingQuest();
            }
        }
    }

    interactText.setFillColor(sf::Color(255, 255, 255, interactTextOpacity));
    interactText.setOutlineColor(sf::Color(0, 0, 0, interactTextOpacity));
}

void Player::handleInput(const float& dt, const sf::RenderWindow& window, std::vector<Npc>& npcs) {
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

    handleQuests(dt, window, npcs);
}

bool Player::isCollisionProjectiles(const sf::FloatRect& rect) {
    for (Projectile& p : projectiles) {
        if (p.isCollision(rect)) {
            p.kill();
            return true;
        }
    }

    return false;
}

bool Player::isCollision(const sf::FloatRect& rect) const {
    return rect.intersects(hitbox.getGlobalBounds());
}

bool Player::isAlive() const {
    return state != static_cast<int>(PlayerState::DYING) && state != static_cast<int>(PlayerState::DEAD);
}

void Player::hurt(const float& damage) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        healthPoints -= damage;

        SoundManager::playSound("playerHurt");

        if (healthPoints <= 0) {
            kill();
        }
    }
}

void Player::knockback(const sf::Vector2f& enemyPosition) {
    if (isAlive() && invincibleCooldownTimer <= 0) {
        movingDirection = (position - enemyPosition);

        movingDirection = Projectile::normalize(movingDirection);

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
        position        = basePosition;
        movingDirection = sf::Vector2f(0.f, 0.f);

        hitbox.setPosition(position);
        loadingBox.setPosition(
            hitbox.getPosition().x + hitbox.getSize().x / 2.f,
            hitbox.getPosition().y + hitbox.getSize().y / 2.f
        );
        
        state = 0;

        healthPoints = maxHealthPoints;

        invincibleCooldownTimer = INVINCIBLE_TIME;
    }
}

sf::FloatRect Player::getHitBox() const {
    return hitbox.getGlobalBounds();
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
    if (interactCooldownTimer > 0) {
        interactCooldownTimer -= dt;
    }
    if (knockbackCooldownTimer > 0) {
        knockbackCooldownTimer -= dt;
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

    sf::FloatRect nextHitboxRect = sf::FloatRect(position, size);
    if (velocity.x != 0) {
        nextHitboxRect.left += velocity.x;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.left -= velocity.x / 10.0f;
            }
        }
    }
    if (velocity.y != 0) {
        nextHitboxRect.top += velocity.y;
        for (const sf::FloatRect& rect : collisionRects) {
            while (rect.intersects(nextHitboxRect)) {
                nextHitboxRect.top -= velocity.y / 10.0f;
            }
        }
    }

    position = nextHitboxRect.getPosition();
}

void Player::updateHitbox() {
    hitbox.setPosition(position);
    loadingBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - LOADING_DISTANCE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - LOADING_DISTANCE
    );
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
    animationManager.setPosition(position - sf::Vector2f(3, 6));
    animationManager.update();

    shadow.setPosition(position + sf::Vector2f(3, size.y - 5));
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
    for (auto it = quests.begin(); it != quests.end(); /**/) {
        if (it->isUpdateStage()) {
            updateQuest = true;
        }
        else if (it->getID() == -1) {
            it->accept();

            ++it;
        }
        else if (it->isCompleted() && !it->isReceiveReward()) {
            updateXP(it->getRewardExp());

            // it = quests.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Player::update(const float& dt, 
                    const sf::RenderWindow& window, 
                    const std::vector<sf::FloatRect>& collisionRects, 
                    std::vector<Npc>& npcs) {

    updateTimer(dt);

    if (!isAlive()) {
        updateHitbox();

        updateAnimation();
        
        respawn();

        return;
    }
    
    handleInput(dt, window, npcs);

    updatePosition(dt, collisionRects);

    updateHitbox();

    updateAnimation();

    updateProjectiles(dt); 

    updateQuests(); 
}

void Player::draw(sf::RenderWindow& window) {
    // window.draw(hitbox);
    // window.draw(loadingBox);

    shadow.draw(window);

    if (invincibleCooldownTimer > 0) {
        animationManager.draw(window, ShaderManager::get("invincible"));
    }
    else if (knockbackCooldownTimer > 0) {
        animationManager.draw(window, ShaderManager::get("flash"));   
    }
    else {
        animationManager.draw(window);
    }

    for (auto& p : projectiles) {
        p.draw(window);
    }

    window.draw(interactText);
}

sf::Vector2f Player::getPosition() const {
    return position;
}

void Player::levelUp() {
    level++;

    maxHealthPoints++;
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

void Player::addVictim(const std::string& label) {
    for (Quest& quest : quests) {
        QuestEventData dataPack;
        dataPack.eventType  = "kill";
        dataPack.targetName = label;
        quest.update(dataPack);
    }

    updateXP(1.0f);
}

float Player::getHealthRatio() const {
    return healthPoints / maxHealthPoints;
}

std::string format2Decimals(float value) {
    std::string str = std::to_string(value);
    return str.substr(0, str.find('.') + 3);
}

std::string Player::getHealthPointsString() const {
    return format2Decimals(healthPoints) + '/' + format2Decimals(maxHealthPoints);
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

const std::vector<Quest>& Player::getQuests() const {
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
    return position + size / 2.0f;
}

void Player::updateView(const float& dt, sf::View& view) const {
    sf::Vector2f currentCenter = view.getCenter();
    sf::Vector2f targetCenter  = position;
    sf::Vector2f lerped        = currentCenter + VIEW_LEAP_SPEED * (targetCenter - currentCenter) * dt;
    lerped.x = std::round(lerped.x);
    lerped.y = std::round(lerped.y);
    view.setCenter(lerped);
}