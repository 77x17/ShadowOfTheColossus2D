#include <SFML/Graphics.hpp>
#include <iostream>

const int windowWidth  = 800;
const int windowHeight = 600;

const int TILE_SIZE    = 32;

class Player {
private:
    // const
    const float MOVE_SPEED = 5.0f;
    const float GROUND     = TILE_SIZE * 10; 
    const float WIDTH      = TILE_SIZE;
    const float HEIGHT     = TILE_SIZE * 1.75;

    // position
    float  x,  y;
    float vx, vy;
    
    sf::RectangleShape shape;

    bool        onGround          = false;
    float       stun              = false;
    const float STUN_THRESHOLD    = 0.2f;
    float       timeSinceLastStun = 0.f;

public:
    Player(int _x = 0, int _y = 0) : x(_x), y(_y), vx(0), vy(0) {
        shape.setSize(sf::Vector2f(WIDTH, HEIGHT));
        shape.setFillColor(sf::Color::Cyan);
        shape.setPosition(x, y);
    }

    void handleInput() {
        vx = 0.f;
        vy = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            vx -= MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            vx += MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            vy += MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            vy -= MOVE_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            
        }
    }

    void update(sf::View& view) {
        x  += vx;
        y  += vy;
        
        shape.setPosition(x, y);

        sf::Vector2f currentCenter = view.getCenter();
        sf::Vector2f targetCenter  = sf::Vector2f(x, y);
        sf::Vector2f lerped        = currentCenter + 0.1f * (targetCenter - currentCenter);

        view.setCenter(lerped);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }

    sf::Vector2f getPosition() const {
        return sf::Vector2f(x, y);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D Pattern Window");
    window.setFramerateLimit(60);

    bool isMinimized = false;

    sf::View view = window.getView();

    Player player;

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

        player.handleInput();

        player.update(view);

        if (isMinimized) {
            // pause rendering or music here if needed
            continue;
        }

        window.clear(sf::Color::White);

        sf::RectangleShape tile;
        tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
        tile.setFillColor(sf::Color::White);
        tile.setOutlineThickness(1);
        tile.setOutlineColor(sf::Color::Black);

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

        player.draw(window);

        window.display();

        window.setView(view);
    }

    return 0;
}