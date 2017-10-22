#include <SFML/Graphics.hpp>

// -- объявления констант --
constexpr unsigned ANTIALIASING_LEVEL = 8;
constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned MAX_FPS = 60;

// -- предварительные объявления функций (всех, кроме main) --
void createWindow(sf::RenderWindow& window);
void initializePackman(sf::CircleShape& shape);
void handleEvents(sf::RenderWindow& window);
void update(sf::Clock& clock, sf::CircleShape& shape);
void render(sf::RenderWindow& window, sf::CircleShape& shape);

// -- определения функций --

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

// Функция инициализирует фигуру пакмана.
void initializePackman(sf::CircleShape& shape)
{
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100, 0);
}

// Функция обрабатывает все события, скопившиеся в очереди событий SFML.
void handleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}

// Функция обновляет фигуру пакмана.
void update(sf::Clock& clock, sf::CircleShape& shape)
{
    const sf::Vector2f speed = { 10, 0 };

    const float elapsedSeconds = clock.restart().asSeconds();
    shape.setPosition(shape.getPosition() + speed * elapsedSeconds);
}

// Функция рисует фигуру пакмана.
void render(sf::RenderWindow& window, sf::CircleShape& shape)
{
    window.clear();
    window.draw(shape);
    window.display();
}

int main(int, char* [])
{
    sf::RenderWindow window;
    createWindow(window);

    sf::CircleShape packman;
    initializePackman(packman);

    sf::Clock clock;
    while (window.isOpen())
    {
        handleEvents(window);
        update(clock, packman);
        render(window, packman);
    }

    return 0;
}
