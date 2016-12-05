#include "gamescene.h"
#include "utils.h"
#include <cassert>

static const sf::Color TRANSPARENT_GRAY = sf::Color(200, 200, 200, 200);

static void InitializeGhostById(std::map<GhostId, Ghost> &ghosts, GhostId ghostId)
{
    // В C++ `operator[]` для контейнеров STL создаёт запись,
    //  если переданного ключа ещё нет в map.
    // В отличии от него, метод `.at()` бросает исключение,
    //  если переданного ключа ещё нет в map.
    Ghost &ghost = ghosts[ghostId];
    initializeGhost(ghost, getGhostStartPosition(ghostId));
}

void initializeGameScene(GameScene &scene, const sf::Vector2f &sceneSize)
{
    bool succeed = scene.arial.loadFromFile("res/arialn.ttf");
    if (!succeed)
    {
        assert(false);
        exit(1);
    }

    initializeField(scene.field);
    initializePackman(scene.packman);
    InitializeGhostById(scene.ghosts, GhostId::BLINKY);
    InitializeGhostById(scene.ghosts, GhostId::PINKY);
    InitializeGhostById(scene.ghosts, GhostId::INKY);
    InitializeGhostById(scene.ghosts, GhostId::CLYDE);

    scene.gameOverBackground.setFillColor(TRANSPARENT_GRAY);
    scene.gameOverBackground.setSize(sceneSize);

    scene.gameOverLabel.setFont(scene.arial);
    scene.gameOverLabel.setColor(sf::Color::Black);
    scene.gameOverLabel.setPosition(0.5f * sceneSize);
    scene.gameOverLabel.setString("Game Over! You lose.");
    utils::CenterizeTextOrigin(scene.gameOverLabel);
}

void updateGameScene(GameScene &scene, float elapsedTime)
{
    if (scene.gameState == GameState::Playing)
    {
        updatePackman(scene.packman, elapsedTime, scene.field);

        // Контейнер std::map при обходе с range-based for
        //  последовательно возвращает пары (ключ, значение),
        //  упакованные в контейнер std::pair.
        // pair имеет тип `std::pair<const GhostId, Ghost> &`
        for (auto &pair : scene.ghosts)
        {
            updateGhost(pair.second, elapsedTime, scene.field);
        }

        const sf::FloatRect packmanBounds = getPackmanBounds(scene.packman);
        for (const auto &pair : scene.ghosts)
        {
            if (getGhostBounds(pair.second).intersects(packmanBounds))
            {
                scene.gameState = GameState::Lose;
            }
        }
    }
}

void drawGameScene(sf::RenderWindow &window, const GameScene &scene)
{
    drawField(window, scene.field);

    // Персонажи рисуются после поля.
    drawPackman(window, scene.packman);

    // pair имеет тип `const std::pair<const GhostId, Ghost> &`
    for (const auto &pair : scene.ghosts)
    {
        drawGhost(window, pair.second);
    }

    if (scene.gameState == GameState::Lose)
    {
        window.draw(scene.gameOverBackground);
        window.draw(scene.gameOverLabel);
    }
}

void destroyGameScene(GameScene &scene)
{
    destroyField(scene.field);
}
