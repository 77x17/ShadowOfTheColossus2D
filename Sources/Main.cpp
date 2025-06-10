#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "Animation.hpp"
#include "Player.hpp"
#include "Bat.hpp"
#include "Eye.hpp"
#include "SoundManager.hpp"
#include "TileMap.hpp"

void drawGrid(const sf::Vector2f& entityPosition, sf::RenderWindow& window) {
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    tile.setFillColor(sf::Color::Transparent);
    tile.setOutlineColor(sf::Color(139, 139, 139, 139));
    tile.setOutlineThickness(2);

    for (int x = 0; x <= TILE_SIZE * 5; x += TILE_SIZE) {
        for (int y = 0; y <= TILE_SIZE * 5; y += TILE_SIZE) {
            sf::Vector2f fixedPosition;
            fixedPosition = sf::Vector2f(entityPosition.x + x, entityPosition.y + y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);
        
            fixedPosition   = sf::Vector2f(entityPosition.x + x, entityPosition.y - y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);

            fixedPosition   = sf::Vector2f(entityPosition.x - x, entityPosition.y + y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);

            fixedPosition   = sf::Vector2f(entityPosition.x - x, entityPosition.y - y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shadow Of The Colossus 2D");
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true); 

    
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

    SoundManager::loadSound("arrow"     , "Sounds/arrow.wav");
    SoundManager::loadSound("playerHurt", "Sounds/playerHurt.wav");
    SoundManager::loadSound("enemyHurt" , "Sounds/enemyHurt.wav");

    bool isMinimized = false;

    sf::View view = window.getView();

    Player player(0, 0);
    std::vector<Bat> bats;
    // bats.push_back(Bat( 300,    0));
    // bats.push_back(Bat(   0,  300));
    // bats.push_back(Bat(-300,    0));
    // bats.push_back(Bat(   0, -300));

    std::vector<Eye> eyes;
    // eyes.push_back(Eye( 300,  300));
    // eyes.push_back(Eye(-300,  300));
    // eyes.push_back(Eye( 300, -300));
    // eyes.push_back(Eye(-300, -300));

    TileMap map;
    if (!map.load("Maps/test.tmx", "Maps/overworld.png")) {
        return -1;
    }
    map.scale(2.f, 2.f);
    map.updateCollisionRects();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::LostFocus) {
                isMinimized = true;
            }
            else if (event.type == sf::Event::GainedFocus) {
                isMinimized = false;
            }
            else if (event.type == sf::Event::Resized) {
                float aspectRatio = static_cast<float>(event.size.width) / event.size.height;
                
                view = window.getView();
                view.setSize(WINDOW_HEIGHT * aspectRatio, WINDOW_HEIGHT);  // giữ chiều cao cố định
                view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        
        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        player.handleInput(window);

        if (player.isAlive()) {
            player.update(view, map.getCollisionRects());
        }
        else {
            player.respawn();
            for (Bat& bat : bats) {
                bat.respawn();
            }
            for (Eye& eye : eyes) {
                eye.respawn();
            }
        }
        for (Bat& bat : bats) if (bat.distance(player) <= LOADING_DISTANCE) {
            bat.update(player);
        }
        for (Eye& eye : eyes) if (eye.distance(player) <= LOADING_DISTANCE) {
            eye.update(player);
        }

        window.clear(sf::Color::White);
        window.draw(map);

        window.setView(view);

        // drawGrid(player.getPosition(), window);
        
        player.draw(window);

        for (Bat& bat : bats) if (bat.distance(player) <= LOADING_DISTANCE) {
            if (bat.isAlive()) {
                bat.draw(window);
            }
            else {
                bat.respawn();
            }
        }
        for (Eye& eye : eyes) if (eye.distance(player) <= LOADING_DISTANCE) {
            if (eye.isAlive()) {
                eye.draw(window);
            }
            else {
                eye.respawn();
            }
        }

        window.display();
    }

    return 0;
}