#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;
using namespace std;

const Time TIME_PER_FRAME = seconds(1.f / 60.f);
const float STEP = 150.f;

const unsigned FIELD_WIDTH = 32;
const unsigned FIELD_HEIGHT = 24;

const float BLOCK_SIZE = 25.f;

void processEvents(RenderWindow & window, Pacman & pacman)
{
	Event event;
	while (window.pollEvent(event))
	{
		// Проверяем нажатую кнопку
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			pacman.direction = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			pacman.direction = DOWN;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			pacman.direction = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			pacman.direction = RIGHT;
		}
		else
		{
			pacman.direction = NONE;
		}

		// Окно закрыли
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void update(Game & game, const Time & deltaTime)
{
	Pacman & pacman = game.pacman;

	Vector2f movement(0.f, 0.f);
	switch (pacman.direction)
	{
	case UP :
		movement.y -= STEP;
		break;
	case DOWN :
		movement.y += STEP;
		break;
	case LEFT :
		movement.x -= STEP;
		break;
	case RIGHT :
		movement.x += STEP;
		break;
	default:
		break;
	}
	pacman.shape->move(movement * deltaTime.asSeconds());
}

void render(RenderWindow & window, Game & game)
{
	window.clear();

	Field & field = game.field;
	for (size_t i = 0; i < field.width * field.height; i++)
	{
		window.draw(field.rects[i]);
	}

	window.draw(*game.pacman.shape);
	window.display();
}

void initializeField(Field & field)
{
	field.width = FIELD_WIDTH;
	field.height = FIELD_HEIGHT;

	char maze[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	field.maze = &maze[0];

	field.rects = new RectangleShape[field.width * field.height];
	for (size_t i = 0; i < field.height; i++)
	{
		for (size_t j = 0; j < field.width; j++)
		{
			RectangleShape & rect = field.rects[i * field.width + j];
			rect.setPosition(BLOCK_SIZE * j, BLOCK_SIZE * i);
			rect.setSize(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			
			if (field.maze[i * field.width + j] == 1)
			{
				rect.setFillColor(Color(163, 58, 3)); // коричневый
			}
			else
			{
				rect.setFillColor(Color(255, 255, 255)); // белый
			}
		}
	}
}

void initializePacman(Pacman & pacman)
{
	pacman.shape = new CircleShape();
	pacman.shape->setRadius(25.f);
	pacman.shape->setPosition(100, 100);
	pacman.shape->setFillColor(Color(255, 216, 0)); // желтый
	pacman.direction = NONE;
}

void initializeGame(Game & game)
{
	initializeField(game.field);
	initializePacman(game.pacman);
}

void destroy(Game & game)
{
	delete [] game.field.rects;
	delete game.pacman.shape;
}

void startGame()
{
	RenderWindow window(VideoMode(800, 600), "Pacman");
	
	Game game;
	initializeGame(game);

	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;

	while (window.isOpen())
	{
		processEvents(window, game.pacman);
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TIME_PER_FRAME)
		{
			timeSinceLastUpdate -= TIME_PER_FRAME;
			processEvents(window, game.pacman);
			update(game, TIME_PER_FRAME);
		}
		render(window, game);
	}
	destroy(game);
}

int main()
{
	startGame();
	return 0;
}

