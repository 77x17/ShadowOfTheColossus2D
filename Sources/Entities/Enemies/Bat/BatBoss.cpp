#include "BatBoss.hpp"

#include "TextureManager.hpp"
#include "Normalize.hpp"

BatBoss::BatBoss(const sf::Vector2f& position, const std::vector<std::pair<float, std::shared_ptr<ItemData>>>& _inventory) 
: Enemy(position, sf::Vector2f(TILE_SIZE, TILE_SIZE) * 2.0f, 100.0f, "Bat Boss Lv.10", _inventory) {
    // --- [Begin] - Configuration ---
    MOVE_SPEED = 100.0f; 

    DETECION_RANGE = 400.0f;
    KNOCKBACK_COOLDOWN = 0.05f;
    KNOCKBACK_STRENGTH = 50.0f;

    ATTACK_RANGE = 100.0f;

    damagePerAttack = 5.0f;
    expAmount       = 50.0f;
    // --- [End] ---

    // --- [Begin] - Animation ---
    detectionBox.setRadius(DETECION_RANGE);
    detectionBox.setPosition(
        hitbox.getPosition().x + hitbox.getSize().x / 2.f - DETECION_RANGE,
        hitbox.getPosition().y + hitbox.getSize().y / 2.f - DETECION_RANGE
    );

    animationManager.addAnimation(static_cast<int>(BatBossState::IDLE_LEFT) , TextureManager::get("batBoss"), 50, 50, 4, 0, 0.5f, false);
    animationManager.addAnimation(static_cast<int>(BatBossState::IDLE_RIGHT), TextureManager::get("batBoss"), 50, 50, 4, 0, 0.5f, true);
    animationManager.addAnimation(static_cast<int>(BatBossState::DYING)     , TextureManager::get("batBossDead"), 50, 50, 1, 0, 0.5f, false);
    
    shadow = Animation(TextureManager::get("batShadow"), 12,  5, 1, 0, 0.0f, false);
    alert  = Animation(TextureManager::get("alert")    ,  8, 10, 1, 0, 0.0f, false);
    // --- [End] ---

    // --- [Begin] - Projectile ---
    SHOOT_COOLDOWN      = 2.0f;
    PROJECTILE_SPEED    = MOVE_SPEED * 1.5;
    PROJECTILE_LIFETIME = 1.0f;
    shootCooldownTimer  = 0.0f;
    // --- [End] ---

    // --- [Begin] - Dash --- 
    DASH_COOLDOWN     = 4.0f;
    DASH_DURATION     = 0.3f;
    dashCooldownTimer = 0.0f;
    dashDuration      = 0.0f;
    // --- [End] ---
}

void BatBoss::respawn() {
    if (state == -1 && respawnCooldownTimer <= 0) {
        state = -3;
    }
}

void BatBoss::followPlayer(const Player& player) {
    Enemy::followPlayer(player);
    
    if (alertCooldownTimer == -13.0f) {
        shootCooldownTimer = SHOOT_COOLDOWN / 2.0f;
        dashCooldownTimer  = DASH_COOLDOWN  / 2.0f;
    }

    if (dashDuration <= 0 && dashCooldownTimer <= 0) {
        dashDuration      = DASH_DURATION;
        dashCooldownTimer = DASH_COOLDOWN;
        dashDirection     = Normalize::normalize(player.getPosition() - hitbox.getPosition());
        
        SoundManager::playSound("dash");
    }
    else if (dashCooldownTimer < 0.2f && dashDuration < 0.2f) {
        alertCooldownTimer = ALERT_LIFETIME;
    }

    if (dashDuration > 0) {
        movingDirection = dashDirection * 5.0f;
    }
    else {
        dashDirection = sf::Vector2f(0.0f, 0.0f);
    }

    if (shootCooldownTimer <= 0) {
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int i = 0; i < 8; i++) {
            projectiles.push_back(std::make_unique<Projectile>(
                TextureManager::get("fireball"),
                hitbox.getPosition() + hitbox.getSize() / 2.f,
                Normalize::normalize(sf::Vector2f(dx[i], dy[i])),
                PROJECTILE_SPEED,
                PROJECTILE_LIFETIME
            ));

            SoundManager::playSound("fireball");
        }

        shootCooldownTimer = SHOOT_COOLDOWN;
    } 
}

void BatBoss::updateTimer(const float& dt) {
    Enemy::updateTimer(dt);

    if (shootCooldownTimer > 0) {
        shootCooldownTimer -= dt;
    }
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    if (dashDuration > 0) {
        dashDuration -= dt;
    }
}

void BatBoss::updateAnimation() {
    if (!isAlive() || knockbackCooldownTimer > 0) {
        // nothing
    }
    else if (movingDirection.x < 0) {
        state = static_cast<int>(BatBossState::IDLE_LEFT);
    }
    else if (movingDirection.x > 0) {
        state = static_cast<int>(BatBossState::IDLE_RIGHT);
    }

    animationManager.setState(state, true);
    animationManager.setPosition(hitbox.getPosition() - sf::Vector2f(18.5f, 20.0f));
    animationManager.update();

    alert .setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2.0f - alert.getSprite().getLocalBounds().width / 2.0f, -hitbox.getSize().y / 2.0f));
    shadow.setPosition(hitbox.getPosition() + sf::Vector2f(hitbox.getSize().x / 2.0f - 10.0f, hitbox.getSize().y));
}

void BatBoss::updateProjectiles(const float& dt, Player& player) {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        (*it)->update(dt);

        bool shouldRemove = false;
        if ((*it)->isAlive()) {
            if ((*it)->isCollision(player.getHitbox())) {
                player.hurt(damagePerAttack);
                player.knockback((*it)->getPosition());

                shouldRemove = true;
            }
        }
        else {
            shouldRemove = true;
        }

        if (shouldRemove) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}   

void BatBoss::update(const float& dt, Player& player, const std::vector<sf::FloatRect>& collisionRects, std::vector<Item>& items) {
    Enemy::update(dt, player,collisionRects, items);
    
    if (invincibleCooldownTimer <= 0) {
        updateProjectiles(dt, player);
    }
}


void BatBoss::draw(sf::RenderTarget& target) {
    for (const std::unique_ptr<Projectile>& projectile : projectiles) {
        projectile->draw(target);
    }
   
    Enemy::draw(target); 
}