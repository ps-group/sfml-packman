#pragma once
#include <SFML/Graphics.hpp>

namespace utils
{
static const float PI = 3.141592654f;

void centerizeTextOrigin(sf::Text &text);
sf::FloatRect uniteRects(const sf::FloatRect &a, const sf::FloatRect &b);
sf::FloatRect moveRectBy(const sf::FloatRect &a, const sf::Vector2f &movement);
float degreesToRadians(float degrees);
sf::Vector2f polarToDecartian(float radius, float degrees);
}
