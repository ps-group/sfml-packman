#include "utils.h"
#include <cmath>

void utils::centerizeTextOrigin(sf::Text &text)
{
    const sf::FloatRect labelBounds = text.getLocalBounds();
    text.setOrigin(0.5f * labelBounds.width,
                   0.5f * labelBounds.height);
}

sf::FloatRect utils::uniteRects(const sf::FloatRect &a, const sf::FloatRect &b)
{
    const float left = std::min(a.left, b.left);
    const float top = std::min(a.top, b.top);
    const float right = std::max(a.left + a.width, b.left + b.width);
    const float bottom = std::max(a.top + a.height, b.top + b.height);

    return sf::FloatRect(left, top, right - left, bottom - top);
}

sf::FloatRect utils::moveRectBy(const sf::FloatRect &a, const sf::Vector2f &movement)
{
    sf::FloatRect result = a;
    result.left += movement.x;
    result.top += movement.y;

    return result;
}

float utils::degreesToRadians(float degrees)
{
    return (degrees * utils::PI / 180);
}

sf::Vector2f utils::polarToDecartian(float radius, float degrees)
{
    const float radians = utils::degreesToRadians(degrees);

    return radius * sf::Vector2f(std::cos(radians), std::sin(radians));
}
