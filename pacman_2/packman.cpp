#include "packman.h"

// Скорость движения, пикселей в секунду.
constexpr float PACKMAN_SPEED = 20.f;

// Радиус фигуры пакмана в пикселях.
constexpr float PACKMAN_RADIUS = 20.f;

// RGB-цвет заливки пакмана/
const sf::Color PACKMAN_COLOR = sf::Color(255, 216, 0);

// Начальная позиция пакмана в пикселях.
static const sf::Vector2f PACKMAN_INITIAL_POSITION = { 100, 0 };

void initializePackman(Packman& packman)
{
    packman.direction = Direction::NONE;
    packman.shape.setRadius(PACKMAN_RADIUS);
    packman.shape.setFillColor(PACKMAN_COLOR);
    packman.shape.setPosition(PACKMAN_INITIAL_POSITION);
}

void updatePackman(Packman& packman, float elapsedTime)
{
    const float step = PACKMAN_SPEED * elapsedTime;
    sf::Vector2f position = packman.shape.getPosition();
    switch (packman.direction)
    {
    case Direction::UP:
        position.y -= step;
        break;
    case Direction::DOWN:
        position.y += step;
        break;
    case Direction::LEFT:
        position.x -= step;
        break;
    case Direction::RIGHT:
        position.x += step;
        break;
    case Direction::NONE:
        break;
    }
    packman.shape.setPosition(position);
}

bool handlePackmanKeyPress(const sf::Event::KeyEvent& event, Packman& packman)
{
    bool handled = true;
    switch (event.code)
    {
    case sf::Keyboard::Up:
        packman.direction = Direction::UP;
        break;
    case sf::Keyboard::Down:
        packman.direction = Direction::DOWN;
        break;
    case sf::Keyboard::Left:
        packman.direction = Direction::LEFT;
        break;
    case sf::Keyboard::Right:
        packman.direction = Direction::RIGHT;
        break;
    default:
        handled = false;
        break;
    }
    return handled;
}

bool handlePackmanKeyRelease(const sf::Event::KeyEvent& event, Packman& packman)
{
    bool handled = true;
    switch (event.code)
    {
    case sf::Keyboard::Up:
        if (packman.direction == Direction::UP)
        {
            packman.direction = Direction::NONE;
        }
        break;
    case sf::Keyboard::Down:
        if (packman.direction == Direction::DOWN)
        {
            packman.direction = Direction::NONE;
        }
        break;
    case sf::Keyboard::Left:
        if (packman.direction == Direction::LEFT)
        {
            packman.direction = Direction::NONE;
        }
        break;
    case sf::Keyboard::Right:
        if (packman.direction == Direction::RIGHT)
        {
            packman.direction = Direction::NONE;
        }
        break;
    default:
        handled = false;
        break;
    }

    return handled;
}
