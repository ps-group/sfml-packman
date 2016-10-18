#include "gamescene.h"

static unsigned getRemainingCookies(const GameScene &scene)
{
    return scene.totalCookieCount - scene.packman.eatenCookies;
}

void initializeGameScene(GameScene &scene)
{
    initializeField(scene.field);
    initializePackman(scene.packman);
    initializeGhost(scene.blinky, getBlinkyStartPosition());
    initializeGhost(scene.pinky, getPinkyStartPosition());
    initializeGhost(scene.inky, getInkyStartPosition());
    initializeGhost(scene.clyde, getClydeStartPosition());
    scene.totalCookieCount = countRemainingCookies(scene.field);
    scene.state = GameState::Playing;
}

void updateGameScene(GameScene &scene, float elapsedTime)
{
    if (scene.state == GameState::Playing)
    {
        updatePackman(scene.packman, elapsedTime, scene.field);
        updateGhost(scene.blinky, elapsedTime, scene.field);
        updateGhost(scene.pinky, elapsedTime, scene.field);
        updateGhost(scene.inky, elapsedTime, scene.field);
        updateGhost(scene.clyde, elapsedTime, scene.field);

        // Проверяем наступление победы.
        if (getRemainingCookies(scene) == 0)
        {
            scene.state = GameState::PlayerWon;
        }
    }
}

std::string getGameSceneWindowTitle(const GameScene &scene)
{
    std::string title;
    if (scene.state == GameState::Playing)
    {
        unsigned cookiesLeft = getRemainingCookies(scene);
        title = "Packman: " + std::to_string(cookiesLeft) + " Cookies Left";
    }
    else
    {
        title = "Packman: Congratulations, You Won!";
    }
    return title;
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
