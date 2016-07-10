#include <SFML/Graphics.hpp>
#include "packman.h"
#include "field.h"

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
    }
}

void update(sf::Clock &clock, Packman &packman)
{
    const float elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    updatePackman(packman, elapsedTime);
}

void render(sf::RenderWindow & window, const Packman &packman, const Field &field)
{
    window.clear();
    drawField(window, field);
    // пакман рисуется после поля.
    window.draw(packman.shape);
    window.display();
}

int main(int, char *[])
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Window Title");
    Packman packman;
    initializePackman(packman);
    Field field;
    initializeField(field);

    sf::Clock clock;

    while (window.isOpen())
    {
        handleEvents(window, packman);
        update(clock, packman);
        render(window, packman, field);
    }

    return 0;
}
