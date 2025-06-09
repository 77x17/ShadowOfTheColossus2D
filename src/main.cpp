#include <SFML/Graphics.hpp>

#include "constants.hpp"
#include "animation.hpp"
#include "player.hpp"
#include "bat.hpp"

void drawGrid(Player& player, sf::RenderWindow& window) {
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    tile.setFillColor(sf::Color::White);
    tile.setOutlineColor(sf::Color(139, 139, 139, 139));
    tile.setOutlineThickness(2);

    sf::Vector2f playerPosition = player.getPosition();
    for (int x = 0; x <= TILE_SIZE * 5; x += TILE_SIZE) {
        for (int y = 0; y <= TILE_SIZE * 5; y += TILE_SIZE) {
            sf::Vector2f fixedPosition;
            fixedPosition = sf::Vector2f(playerPosition.x + x, playerPosition.y + y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);
        
            fixedPosition   = sf::Vector2f(playerPosition.x + x, playerPosition.y - y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);

            fixedPosition   = sf::Vector2f(playerPosition.x - x, playerPosition.y + y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);

            fixedPosition   = sf::Vector2f(playerPosition.x - x, playerPosition.y - y);
            fixedPosition.x = static_cast<int>(fixedPosition.x / TILE_SIZE) * TILE_SIZE;
            fixedPosition.y = static_cast<int>(fixedPosition.y / TILE_SIZE) * TILE_SIZE;
            tile.setPosition(fixedPosition);
            window.draw(tile);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Pattern Window");
    window.setFramerateLimit(60);

    TextureManager::load("playerSprite", "sprites/player.png");
    TextureManager::load("playerShadow", "sprites/playerShadow.png");
    TextureManager::load("arrow", "sprites/arrow.png");

    TextureManager::load("alert", "sprites/alert.png");
    TextureManager::load("batSprite", "sprites/bat.png");
    TextureManager::load("batShadow", "sprites/batShadow.png");

    bool isMinimized = false;

    sf::View view = window.getView();

    Player player(0, 0);
    std::vector<Bat> bats;
    bats.push_back(Bat(251, 0));
    bats.push_back(Bat(0, 251));
    bats.push_back(Bat(-251, 0));
    bats.push_back(Bat(0, -251));

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
                view.setSize(windowHeight * aspectRatio, windowHeight);  // giữ chiều cao cố định
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

        player.update(view);
        for (Bat& bat : bats) {
            bat.update(player);
        }

        window.clear(sf::Color::White);

        window.setView(view);

        drawGrid(player, window);

        player.draw(window);
        for (Bat& bat : bats) {
            bat.draw(window);
        }

        window.display();
    }

    return 0;
}