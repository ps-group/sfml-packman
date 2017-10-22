#include "gamescene.h"
#include "utils.h"
#include <cassert>

static const sf::Color TRANSPARENT_GRAY = sf::Color(200, 200, 200, 200);

static const std::map<GhostId, std::string> TEXTURE_MAPPING = {
    { GhostId::BLINKY, RED_GHOST_TEXTURE },
    { GhostId::PINKY, PINK_GHOST_TEXTURE },
    { GhostId::INKY, BLUE_GHOST_TEXTURE },
    { GhostId::CLYDE, ORANGE_GHOST_TEXTURE },
};

static bool initializeGhostById(std::map<GhostId, Ghost> &ghosts, GhostId id)
{
    // В C++ `operator[]` для контейнеров STL создаёт запись,
    //  если переданного ключа ещё нет в map.
    Ghost &ghost = ghosts[id];
    // В отличии от `operator[]`, метод `.at()` бросает исключение,
    //  если переданного ключа нет в map.
    const std::string texturePath = TEXTURE_MAPPING.at(id);

    return initializeGhost(ghost, getGhostStartPosition(id), texturePath);
}

static unsigned getRemainingCookies(const GameScene &scene)
{
    return scene.totalCookieCount - scene.packman.eatenCookies;
}

static void updateGameOverLabel(sf::Text &label, const std::string &text)
{
    label.setString(text);
    utils::centerizeTextOrigin(label);
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

    succeed  = initializeGhostById(scene.ghosts, GhostId::BLINKY)
            && initializeGhostById(scene.ghosts, GhostId::PINKY)
            && initializeGhostById(scene.ghosts, GhostId::INKY)
            && initializeGhostById(scene.ghosts, GhostId::CLYDE);
    if (!succeed)
    {
        assert(false);
        exit(1);
    }

    scene.gameState = GameState::Playing;
    scene.totalCookieCount = countRemainingCookies(scene.field);

    scene.gameOverBackground.setFillColor(TRANSPARENT_GRAY);
    scene.gameOverBackground.setSize(sceneSize);

    scene.gameOverLabel.setFont(scene.arial);
    scene.gameOverLabel.setFillColor(sf::Color::Black);
    scene.gameOverLabel.setPosition(0.5f * sceneSize);
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

        // Проверяем условие поражения - столкновение пакмана и призрака.
        const sf::FloatRect packmanBounds = getPackmanBounds(scene.packman);
        for (const auto &pair : scene.ghosts)
        {
            if (getGhostBounds(pair.second).intersects(packmanBounds))
            {
                updateGameOverLabel(scene.gameOverLabel, "Game Over! You lose.");
                scene.gameState = GameState::PlayerLosed;
            }
        }

        // Проверяем условие победы - всё печенье съедено.
        if (getRemainingCookies(scene) == 0)
        {
            updateGameOverLabel(scene.gameOverLabel, "Congratulations, you won!");
            scene.gameState = GameState::PlayerWon;
        }
    }
}

std::string getGameSceneWindowTitle(const GameScene &scene)
{
    std::string title;
    switch (scene.gameState)
    {
    case GameState::Playing:
    {
        unsigned cookiesLeft = getRemainingCookies(scene);
        title = "Packman: " + std::to_string(cookiesLeft) + " Cookies Left";
    }
        break;
    case GameState::PlayerWon:
        title = "Packman: Congratulations, You Won!";
        break;
    case GameState::PlayerLosed:
        title = "Packman: Unfortunately, You Lose";
        break;
    }

    return title;
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

    if ((scene.gameState == GameState::PlayerLosed)
            || (scene.gameState == GameState::PlayerWon))
    {
        window.draw(scene.gameOverBackground);
        window.draw(scene.gameOverLabel);
    }
}

void destroyGameScene(GameScene &scene)
{
    destroyField(scene.field);
}
