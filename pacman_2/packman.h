#pragma once

#include <SFML/Graphics.hpp>

// Направление движения (не движется, вверх, вниз, влево, вправо)
enum struct Direction
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// Структура описывает пакмана
// Пакмана можно смоделировать фигурой и направлением движения.
struct Packman
{
    sf::CircleShape shape;
    Direction direction;
};

void initializePackman(Packman& packman);
void updatePackman(Packman& packman, float elapsedTime);
bool handlePackmanKeyPress(const sf::Event::KeyEvent& event, Packman& packman);
bool handlePackmanKeyRelease(const sf::Event::KeyEvent& event, Packman& packman);
