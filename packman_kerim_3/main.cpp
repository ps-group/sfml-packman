#include <SFML/Graphics.hpp>
#include "Game.h"

using namespace sf;
using namespace std;

const Time TIME_PER_FRAME = seconds(1.f / 60.f);
const float STEP = 200.f;

const unsigned FIELD_WIDTH = 32;
const unsigned FIELD_HEIGHT = 24;

const float BLOCK_SIZE = 25.f;

void processEvents(RenderWindow & window, Pacman * pacman)
{
	Event event;
	while (window.pollEvent(event))
	{
		// Проверяем нажатую кнопку
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			pacman->direction = UP;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			pacman->direction = DOWN;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			pacman->direction = LEFT;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			pacman->direction = RIGHT;
		}

		// Окно закрыли
		if (event.type == Event::Closed)
		{
			window.close();
		}
	}
}

void checkCollision(Game * game) 
{
	Pacman * pacman = game->pacman;
	CircleShape * pacmanInstance = pacman->instance;
	FloatRect pacmanBound = pacmanInstance->getGlobalBounds();

	Field * field = game->field;
	Cell * cells = field->cells;

	for (size_t i = 0; i < field->width * field->height; i++)
	{
		if (cells[i].type == FREE_SPACE)
		{
			continue;
		}

		Vector2f position = pacmanInstance->getPosition();
		FloatRect blockBound = cells[i].instance->getGlobalBounds();
		if (pacmanBound.intersects(blockBound))
		{
			switch (pacman->direction)
			{
			case UP:
				position.y += blockBound.top + blockBound.height - pacmanBound.top;
				break;
			case DOWN:
				position.y -= pacmanBound.top + pacmanBound.height - blockBound.top;
				break;
			case LEFT:
				position.x += blockBound.left + blockBound.width - pacmanBound.left;
				break;
			case RIGHT:
				position.x -= pacmanBound.left + pacmanBound.width - blockBound.left;
				break;
			}
			pacmanInstance->setPosition(position);
			pacman->direction = NONE;
		}
	}
}

void update(Game * game, const Time & deltaTime)
{
	Pacman * pacman = game->pacman;

	Vector2f movement(0.f, 0.f);
	switch (pacman->direction)
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

	pacman->instance->move(movement * deltaTime.asSeconds());
	checkCollision(game);
}

void render(RenderWindow & window, Game * game)
{
	window.clear();

	Field * field = game->field;
	Cell * cell = field->cells;
	for (size_t i = 0; i < field->width * field->height; i++)
	{
		window.draw(*cell[i].instance);
	}

	Pacman * pacman = game->pacman;
	window.draw(*pacman->instance);
	window.display();
}

void initializeField(Field * &field)
{
	char maze[] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	field = new Field();
	
	field->width = FIELD_WIDTH;
	field->height = FIELD_HEIGHT;

	Cell * &cells = field->cells;
	cells = new Cell[field->width * field->height];
	for (size_t i = 0; i < field->height; i++)
	{
		for (size_t j = 0; j < field->width; j++)
		{
			const size_t index = i * field->width + j;
			RectangleShape * &instance = cells[index].instance;

			instance = new RectangleShape();
			instance->setPosition(BLOCK_SIZE * j, BLOCK_SIZE * i);
			instance->setSize(Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			if (maze[index] == 1)
			{
				cells[index].type = BLOCK;
				instance->setFillColor(Color(163, 58, 3)); // коричневый
			}
			else
			{
				cells[index].type = FREE_SPACE;
				instance->setFillColor(Color(255, 255, 255)); // белый
			}
		}
	}
}

void initializePacman(Pacman * &pacman)
{
	pacman = new Pacman();
	CircleShape * &instance = pacman->instance;
	instance = new CircleShape();
	instance->setRadius(25.f);
	instance->setPosition(200, 200);
	instance->setFillColor(Color(255, 216, 0)); // желтый
	pacman->direction = NONE;
}

void initializeGame(Game * &game)
{
	game = new Game();
	initializeField(game->field);
	initializePacman(game->pacman);
}

void destroyField(Field * field)
{
	Cell * cells = field->cells;
	for (size_t i = 0; i < field->width * field->height; i++)
	{
		delete cells[i].instance;
	}
	delete[] field->cells;
	delete field;
}

void destroyPacman(Pacman * pacman)
{
	delete pacman->instance;
	delete pacman;
}

void destroyGame(Game * game)
{	
	destroyField(game->field);
	destroyPacman(game->pacman);
	delete game;
}

void startGame()
{
	RenderWindow window(VideoMode(800, 600), "Pacman");
	
	Game *game;
	initializeGame(game);

	Clock clock;
	Time timeSinceLastUpdate = Time::Zero;

	while (window.isOpen())
	{
		processEvents(window, game->pacman);
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > TIME_PER_FRAME)
		{
			timeSinceLastUpdate -= TIME_PER_FRAME;
			processEvents(window, game->pacman);
			update(game, TIME_PER_FRAME);
		}
		render(window, game);
	}
	destroyGame(game);
}

int main()
{
	startGame();
	return 0;
}