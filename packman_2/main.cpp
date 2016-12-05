#include <SFML/Graphics.hpp>
#include "packman.h"

void handleEvents(sf::RenderWindow & window, Packman &packman)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Кнопка закрытия окна
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        // Клавиши управления пакманом
        else if (event.type == sf::Event::KeyPressed)
        {
            handlePackmanKeyPress(event.key, packman);
        }
        else if (event.type == sf::Event::KeyReleased)
        {
            handlePackmanKeyRelease(event.key, packman);
        }
    }
}

void update(sf::Clock &clock, Packman &packman)
{
    const float elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    updatePackman(packman, elapsedTime);
}

void render(sf::RenderWindow & window, const Packman &packman)
{
    window.clear();
    window.draw(packman.shape);
    window.display();
}

int main(int, char *[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Packman Game Clone", sf::Style::Close, settings);

    Packman packman;
    initializePackman(packman);

    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents(window, packman);
        update(clock, packman);
        render(window, packman);
    }

    return 0;
}
