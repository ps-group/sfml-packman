#pragma once

#include <SFML/Graphics.hpp>
#include "packman.h"

struct Ghost
{
    sf::CircleShape shape;
    Direction direction;
};

void initializeGhost(Ghost & ghost, const sf::Vector2f &position);
void updateGhost(Ghost &ghost, float elapsedTime, const Field &field);
void drawGhost(sf::RenderWindow &window, const Ghost &ghost);
