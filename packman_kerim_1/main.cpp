#include <SFML/Graphics.hpp>

using namespace sf;

typedef enum 
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

void processEvents(RenderWindow & window, Direction & direction)
{
	Event event;
	while (window.pollEvent(event))
	{
		// Проверяем нажатую кнопку
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			direction = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			direction = DOWN;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			direction = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			direction = RIGHT;
		}
		else
		{
			direction = NONE;
		}

		// Окно закрыли
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void update(CircleShape & shape, Direction & direction)
{
	const float STEP = 0.2f;

	Vector2f position = shape.getPosition();
	switch (direction)
	{
	case UP :
		position.y -= STEP;
		break;
	case DOWN :
		position.y += STEP;
		break;
	case LEFT :
		position.x -= STEP;
		break;
	case RIGHT :
		position.x += STEP;
		break;
	default:
		break;
	}
	shape.setPosition(position);
}

void render(RenderWindow & window, CircleShape & shape)
{
	window.clear();
	window.draw(shape);
	window.display();
}

void startGame()
{
	RenderWindow window(VideoMode(800, 600), "Pacman");
	
	CircleShape shape(25.f);
	shape.setFillColor(Color(255, 216, 0)); // желтый цвет
	shape.setPosition(100, 100);

	Direction direction = NONE;

	while (window.isOpen())
	{
		processEvents(window, direction);
		update(shape, direction);
		render(window, shape);
	}
}

int main()
{
	startGame();
	return 0;
}

