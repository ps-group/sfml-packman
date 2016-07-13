#include "gamescene.h"

void initializeGameScene(GameScene &scene)
{
    initializeField(scene.field);
    initializePackman(scene.packman);
    initializeGhost(scene.blinky, getBlinkyStartPosition());
    initializeGhost(scene.pinky, getPinkyStartPosition());
    initializeGhost(scene.inky, getInkyStartPosition());
    initializeGhost(scene.clyde, getClydeStartPosition());
}

void updateGameScene(GameScene &scene, float elapsedTime)
{
    updatePackman(scene.packman, elapsedTime, scene.field);
    updateGhost(scene.blinky, elapsedTime, scene.field);
    updateGhost(scene.pinky, elapsedTime, scene.field);
    updateGhost(scene.inky, elapsedTime, scene.field);
    updateGhost(scene.clyde, elapsedTime, scene.field);
}

void drawGameScene(sf::RenderWindow &window, const GameScene &scene)
{
    drawField(window, scene.field);
    // Персонажи рисуются после поля.
    drawPackman(window, scene.packman);
    drawGhost(window, scene.blinky);
    drawGhost(window, scene.pinky);
    drawGhost(window, scene.inky);
    drawGhost(window, scene.clyde);
}

void destroyGameScene(GameScene &scene)
{
    destroyField(scene.field);
}
