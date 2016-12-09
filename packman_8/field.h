#pragma once
#include <SFML/Graphics.hpp>

enum class GhostId
{
    BLINKY,
    PINKY,
    INKY,
    CLYDE,
};

enum struct CellCategory
{
    WALL,
    EMPTY,
    COOKIE,
    SUPERCOOKIE,
};

struct Cell
{
    CellCategory category;
    sf::FloatRect bounds;
};

struct Field
{
    size_t width = 0;
    size_t height = 0;
    Cell *cells = nullptr;
};

sf::Vector2f getPackmanStartPosition();
sf::Vector2f getGhostStartPosition(GhostId ghostId);

// Возвращает количество оставшегося печенья.
unsigned countRemainingCookies(const Field &field);

// Возвращает количество поглощённого печенья.
unsigned eatAllCookiesInBounds(Field &field, const sf::FloatRect &bounds);

void initializeField(Field &field);
void drawField(sf::RenderWindow &window, const Field &field);
bool checkFieldWallsCollision(const Field &field, const sf::FloatRect &oldBounds, sf::Vector2f &movement);
void destroyField(Field &field);
