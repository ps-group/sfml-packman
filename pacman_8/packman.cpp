#include "packman.h"
#include "field.h"
#include "utils.h"
#include <math.h>

static const sf::Color PACKMAN_COLOR = sf::Color(255, 216, 0);
static const float PACKMAN_SPEED = 120.f; // pixels per second.
static const float PACKMAN_RADIUS = 16.0f; // pixels
static const float PACKMAN_VISIBLE_RADIUS = 12.0f; // pixels
static const float PACKMAN_MOUTH_ANGLE = 110.f; // degrees.
static const float PACKMAN_ANIMATION_PERIOD = 0.3f;

/// angle - polar angle in degrees
static sf::Vector2f getRadialPoint(float angle, float radius)
{
    return utils::polarToDecartian(radius, angle - 90);
}

static void assignShape(sf::ConvexShape & shape, std::vector<sf::Vector2f> const& points)
{
    const unsigned pointCount = unsigned(points.size());
    shape.setPointCount(pointCount);
    for (unsigned i = 0; i < pointCount; ++i)
    {
        shape.setPoint(i, points[i]);
    }
}

static void assignPackmanShape(sf::ConvexShape & topShape,
                               sf::ConvexShape & bottomShape,
                               const float animationPhase)
{
    // Вычисляем половину угла раскрытого рта персонажа packman.
    const float deviationPhase = 2.f * fabsf(0.5f - animationPhase);
    const float deviationAngle = 0.5 * deviationPhase * PACKMAN_MOUTH_ANGLE;

    const float radius = PACKMAN_VISIBLE_RADIUS;
    std::vector<sf::Vector2f> points;
    sf::Vector2f center(0, 0.25f * radius);

    for (float angle = 180.f; angle >= deviationAngle; angle -= 5.f)
    {
        points.push_back(getRadialPoint(angle, radius));
    }
    points.push_back(center);
    assignShape(topShape, points);
    points.clear();

    for (float angle = 180.f; angle <= 360.f - deviationAngle; angle += 5.f)
    {
        points.push_back(getRadialPoint(angle, radius));
    }
    points.push_back(center);
    assignShape(bottomShape, points);
}

static void updatePackmanDirection(Packman &packman)
{
    packman.direction = Direction::NONE;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        packman.direction = Direction::UP;
        packman.orientationDegrees = 0;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
             || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        packman.direction = Direction::DOWN;
        packman.orientationDegrees = 180;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        packman.direction = Direction::LEFT;
        packman.orientationDegrees = 270;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
             || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        packman.direction = Direction::RIGHT;
        packman.orientationDegrees = 90;
    }
}

void initializePackman(Packman &packman)
{
    packman.direction = Direction::NONE;
    packman.position = getPackmanStartPosition();
    packman.eatenCookies = 0;
    packman.orientationDegrees = 90;
    packman.animationPhase = 0.25;

    packman.topShape.setFillColor(sf::Color::Yellow);
    packman.bottomShape.setFillColor(sf::Color::Yellow);
}

void updatePackman(Packman &packman, float elapsedTime, Field &field)
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
    if (checkFieldWallsCollision(field, getPackmanBounds(packman), movement))
    {
        // Останавливаем пакмана при столкновении
        packman.direction = Direction::NONE;
    }
    packman.eatenCookies += eatAllCookiesInBounds(field, getPackmanBounds(packman));
    packman.position += movement;

    // Обновляем фазу анимации персонажа
    //  (анимация продолжается только в движении)
    if (packman.direction == Direction::NONE)
    {
        packman.animationPhase = 0.25;
    }
    else
    {
        const float deltaPhase = elapsedTime / PACKMAN_ANIMATION_PERIOD;
        packman.animationPhase = fmodf(packman.animationPhase + deltaPhase, 1.f);
    }

    // Устанавливаем границы фигуры пакмана.
    assignPackmanShape(packman.topShape, packman.bottomShape, packman.animationPhase);
}

void drawPackman(sf::RenderWindow &window, const Packman &packman)
{
    // Настраиваем 2D трансформацию для рисования двух половин пакмана.
    sf::RenderStates states;
    // Применяем перемещение.
    states.transform.translate(packman.position);
    // Применяем вращение вокруг собственного центра.
    states.transform.rotate(packman.orientationDegrees);

    window.draw(packman.topShape, states);
    window.draw(packman.bottomShape, states);
}

sf::FloatRect getPackmanBounds(const Packman &packman)
{
    return sf::FloatRect(packman.position.x - PACKMAN_RADIUS,
                         packman.position.y - PACKMAN_RADIUS,
                         2.f * PACKMAN_RADIUS,
                         2.f * PACKMAN_RADIUS);
}
