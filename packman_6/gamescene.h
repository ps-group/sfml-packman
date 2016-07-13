#pragma once

#include "field.h"
#include "ghost.h"
#include "packman.h"

struct GameScene
{
    Field field;
    Packman packman;
    Ghost blinky;
    Ghost pinky;
    Ghost inky;
    Ghost clyde;
};

void initializeGameScene(GameScene &scene);
void updateGameScene(GameScene &scene, float elapsedTime);
void drawGameScene(sf::RenderWindow &window, const GameScene &scene);
void destroyGameScene(GameScene &scene);
