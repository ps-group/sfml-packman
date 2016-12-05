#include <SFML/Graphics.hpp>

void initializePackman(sf::CircleShape & shape)
{
    shape.setRadius(20);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100, 0);
}

void handleEvents(sf::RenderWindow & window)
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

void render(sf::RenderWindow & window, sf::CircleShape & shape)
{
    window.clear();
    window.draw(shape);
    window.display();
}

int main(int, char *[])
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Packman Game Clone");
    sf::CircleShape packman;
    initializePackman(packman);

    while (window.isOpen())
    {
        handleEvents(window);
        render(window, packman);
    }

    return 0;
}
