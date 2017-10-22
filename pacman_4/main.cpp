#include "field.h"
#include "packman.h"
#include <SFML/Graphics.hpp>

void handleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Кнопка закрытия окна
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

void update(sf::Clock& clock, Packman& packman, const Field& field)
{
    const float elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    updatePackman(packman, elapsedTime, field);
}

void render(sf::RenderWindow& window, const Packman& packman, const Field& field)
{
    window.clear();
    drawField(window, field);
    // пакман рисуется после поля.
    window.draw(packman.shape);
    window.display();
}

int main(int, char* [])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(800, 600), "PacMan Game Clone", sf::Style::Close, settings);

    Packman packman;
    initializePackman(packman);
    Field field;
    initializeField(field);

    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents(window);
        update(clock, packman, field);
        render(window, packman, field);
    }

    return 0;
}
