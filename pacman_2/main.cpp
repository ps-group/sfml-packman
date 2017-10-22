#include "packman.h"
#include <SFML/Graphics.hpp>

// -- объявления констант --
constexpr unsigned ANTIALIASING_LEVEL = 8;
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned MAX_FPS = 60;

// Функция создаёт окно приложения.
void createWindow(sf::RenderWindow& window)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
    window.create(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "PacMan Game Clone", sf::Style::Default, settings);
    window.setFramerateLimit(MAX_FPS);
}

// Функция обрабатывает все события, скопившиеся в очереди событий SFML.
void handleEvents(sf::RenderWindow& window, Packman& packman)
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

// Функция обновляет состояние объектов на сцене.
void update(sf::Clock& clock, Packman& packman)
{
    const float elapsedTime = clock.getElapsedTime().asSeconds();
    clock.restart();
    updatePackman(packman, elapsedTime);
}

// Функция рисует объекты на сцене.
void render(sf::RenderWindow& window, const Packman& packman)
{
    window.clear();
    window.draw(packman.shape);
    window.display();
}

int main(int, char* [])
{
    sf::RenderWindow window;
    createWindow(window);

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
