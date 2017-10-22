#include "ghost.h"
#include "field.h"

static const float GHOST_SPEED = 60.f; // pixels per second.
static const float GHOST_SIZE = 32.f; // pixels
static const sf::IntRect FRAME_EYES_LEFT(0, 0, 32, 32);
static const sf::IntRect FRAME_EYES_CENTER(32, 0, 32, 32);
static const sf::IntRect FRAME_EYES_RIGHT(64, 0, 32, 32);

// Размещаем переменные, указанные в заголовке как `extern`.
const char BLUE_GHOST_TEXTURE[] = "res/ghost-blue.png";
const char PINK_GHOST_TEXTURE[] = "res/ghost-pink.png";
const char RED_GHOST_TEXTURE[] = "res/ghost-red.png";
const char ORANGE_GHOST_TEXTURE[] = "res/ghost-orange.png";

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

bool initializeGhost(Ghost &ghost, const sf::Vector2f &position, const std::string &texturePath)
{
    if (!ghost.texture.loadFromFile(texturePath))
    {
        return false;
    }

    ghost.direction = Direction::NONE;
    ghost.shape.setSize({GHOST_SIZE, GHOST_SIZE});
    ghost.shape.setPosition(position);
    ghost.shape.setTexture(&ghost.texture);
    ghost.shape.setTextureRect(FRAME_EYES_CENTER);

    return true;
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
    switch (ghost.direction)
    {
    case Direction::LEFT:
        ghost.shape.setTextureRect(FRAME_EYES_LEFT);
        break;
    case Direction::RIGHT:
        ghost.shape.setTextureRect(FRAME_EYES_RIGHT);
        break;
    case Direction::DOWN:
    case Direction::UP:
    case Direction::NONE:
        ghost.shape.setTextureRect(FRAME_EYES_CENTER);
        break;
    }
}

void drawGhost(sf::RenderWindow &window, const Ghost &ghost)
{
    window.draw(ghost.shape);
}

sf::FloatRect getGhostBounds(const Ghost &ghost)
{
    return ghost.shape.getGlobalBounds();
}
