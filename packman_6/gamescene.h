#pragma once

#include "field.h"
#include "ghost.h"
#include "packman.h"

enum struct GameState
{
	Playing,
	PlayerWon,
};

struct GameScene
{
    Field field;
    Packman packman;
    Ghost blinky;
    Ghost pinky;
    Ghost inky;
    Ghost clyde;
	unsigned totalCookieCount;
	GameState state;
};

void initializeGameScene(GameScene &scene);
void updateGameScene(GameScene &scene, float elapsedTime);
std::string getGameSceneWindowTitle(const GameScene &scene);
void drawGameScene(sf::RenderWindow &window, const GameScene &scene);
void destroyGameScene(GameScene &scene);
