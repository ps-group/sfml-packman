#pragma once

#include <SFML/Graphics.hpp>

struct Field;

enum struct Direction
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct Packman
{
    sf::CircleShape shape;
    Direction direction;
};

void initializePackman(Packman & packman);
void updatePackman(Packman & packman, float elapsedTime, const Field &field);
void drawPackman(sf::RenderWindow &window, const Packman &packman);
sf::FloatRect getPackmanBounds(const Packman &packman);
