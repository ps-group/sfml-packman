#pragma once

#include "field.h"
#include "ghost.h"
#include "packman.h"
#include <map>

enum class GameState
{
    Playing,
    PlayerLosed,
    PlayerWon,
};

struct GameScene
{
    Field field;
    Packman packman;
    std::map<GhostId, Ghost> ghosts;
    GameState gameState;
    unsigned totalCookieCount;

    sf::Font arial;
    sf::RectangleShape gameOverBackground;
    sf::Text gameOverLabel;
};

void initializeGameScene(GameScene &scene, const sf::Vector2f &sceneSize);
void updateGameScene(GameScene &scene, float elapsedTime);
std::string getGameSceneWindowTitle(const GameScene &scene);
void drawGameScene(sf::RenderWindow &window, const GameScene &scene);
void destroyGameScene(GameScene &scene);
