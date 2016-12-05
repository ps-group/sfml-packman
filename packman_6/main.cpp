#include <SFML/Graphics.hpp>
#include "gamescene.h"

void handleEvents(sf::RenderWindow & window)
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

void update(sf::Clock &clock, GameScene &scene)
{
    const float elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    updateGameScene(scene, elapsedTime);
}

void render(sf::RenderWindow & window, const GameScene &scene)
{
    window.clear();
    drawGameScene(window, scene);
    window.display();
}

int main(int, char *[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Packman Game Clone", sf::Style::Close, settings);

    sf::Clock clock;
    GameScene scene;
    initializeGameScene(scene);

    while (window.isOpen())
    {
        handleEvents(window);
        update(clock, scene);
        render(window, scene);
    }

    return 0;
}
