#include "packman.h"
#include "field.h"

static const sf::Color PACKMAN_COLOR = sf::Color(255, 216, 0);
static const float PACKMAN_SPEED = 120.f; // pixels per second.
static const float PACKMAN_RADIUS = 16.f; // pixels

static void updatePackmanDirection(Packman &packman)
{
    packman.direction = Direction::NONE;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        packman.direction = Direction::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
             || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        packman.direction = Direction::DOWN;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        packman.direction = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
             || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        packman.direction = Direction::RIGHT;
    }
}

void initializePackman(Packman &packman)
{
    packman.direction = Direction::NONE;
    packman.shape.setRadius(PACKMAN_RADIUS);
    packman.shape.setFillColor(PACKMAN_COLOR);
    packman.shape.setPosition(getPackmanStartPosition());
}

void updatePackman(Packman &packman, float elapsedTime, const Field &field)
{
    const float step = PACKMAN_SPEED * elapsedTime;

    updatePackmanDirection(packman);

    sf::Vector2f movement(0.f, 0.f);
    switch (packman.direction)
    {
    case Direction::UP:
        movement.y -= step;
        break;
    case Direction::DOWN:
        movement.y += step;
        break;
    case Direction::LEFT:
        movement.x -= step;
        break;
    case Direction::RIGHT:
        movement.x += step;
        break;
    case Direction::NONE:
        break;
    }
    const sf::FloatRect packmanBounds = packman.shape.getGlobalBounds();
    if (checkFieldWallsCollision(field, packmanBounds, movement))
    {
        // Останавливаем пакмана при столкновении
        packman.direction = Direction::NONE;
    }
    packman.shape.move(movement);
}

void drawPackman(sf::RenderWindow &window, const Packman &packman)
{
    window.draw(packman.shape);
}

sf::FloatRect getPackmanBounds(const Packman &packman)
{
    return packman.shape.getGlobalBounds();
}
