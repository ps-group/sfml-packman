#include "ghost.h"
#include "field.h"

static const sf::Color GHOST_COLOR = sf::Color(200, 32, 200);
static const float GHOST_SPEED = 60.f; // pixels per second.
static const float GHOST_RADIUS = 12.5f; // pixels

static void changeGhostDirection(Ghost &ghost)
{
    switch (ghost.direction)
    {
    case Direction::UP:
        ghost.direction = Direction::DOWN;
        break;
    case Direction::DOWN:
        ghost.direction = Direction::LEFT;
        break;
    case Direction::LEFT:
        ghost.direction = Direction::RIGHT;
        break;
    case Direction::RIGHT:
        ghost.direction = Direction::NONE;
        break;
    case Direction::NONE:
        ghost.direction = Direction::UP;
        break;
    }
}

void initializeGhost(Ghost &ghost, const sf::Vector2f &position)
{
    ghost.direction = Direction::NONE;
    ghost.shape.setRadius(GHOST_RADIUS);
    ghost.shape.setFillColor(GHOST_COLOR);
    ghost.shape.setPosition(position);
}

void updateGhost(Ghost &ghost, float elapsedTime, const Field &field)
{
    const float step = GHOST_SPEED * elapsedTime;

    sf::Vector2f movement(0.f, 0.f);
    switch (ghost.direction)
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
        changeGhostDirection(ghost);
        break;
    }
    const sf::FloatRect ghostBounds = ghost.shape.getGlobalBounds();
    if (checkFieldWallsCollision(field, ghostBounds, movement))
    {
        // Меняем направление противника при столкновении со стеной
        changeGhostDirection(ghost);
    }
    ghost.shape.move(movement);
}

void drawGhost(sf::RenderWindow &window, const Ghost &ghost)
{
    window.draw(ghost.shape);
}
