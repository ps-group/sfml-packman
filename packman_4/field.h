#pragma once
#include <SFML/Graphics.hpp>

struct Field
{
    size_t width = 0;
    size_t height = 0;
    sf::RectangleShape *rects = nullptr;
};

void initializeField(Field &field);
void destroyField(Field &field);
void drawField(sf::RenderWindow &window, const Field &field);
