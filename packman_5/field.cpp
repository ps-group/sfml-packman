#include "field.h"
#include "packman.h"
#include <cmath>
#include <limits>

static const float BLOCK_SIZE = 32.f;
static const float EPSILON = std::numeric_limits<float>::epsilon();
static const float MAX_SHIFT = 0.5f * BLOCK_SIZE;
static const size_t FIELD_WIDTH = 25;
static const size_t FIELD_HEIGHT = 25;

static const char FIELD_MAZE[] =
        " ####################### "
        " #          #          # "
        " # ## ##### # ##### ## # "
        " #                     # "
        " # ## # ######### # ## # "
        " #    #     #     #    # "
        " #### ##### # ##### #### "
        "    # #     C     # #    "
        "##### # # ##### # # #####"
        "#       # #BPI# #       #"
        "##### # # ##### # # #####"
        "    # #           # #    "
        " #### # ######### # #### "
        " #          #          # "
        " # ## ##### # ##### ## # "
        " #  #           @   #  # "
        " ## # # ######### # #  # "
        " #    #     #     #    # "
        " # ####### ### ####### # "
        " # #     #     #     # # "
        " # # ### ## # ## ### # # "
        " # # #      #      # # # "
        " # # # #### # #### # # # "
        " #          #          # "
        " ####################### ";

static const sf::Color BROWN_COLOR = sf::Color(163, 58, 3);
static const sf::Color WHITE_COLOR = sf::Color(255, 255, 255);

static sf::FloatRect moveRect(const sf::FloatRect &rect, sf::Vector2f &offset)
{
    return {rect.left + offset.x, rect.top + offset.y, rect.width, rect.height};
}

static float getBottom(const sf::FloatRect &rect)
{
    return rect.top + rect.height;
}

static float getRight(const sf::FloatRect &rect)
{
    return rect.left + rect.width;
}

static bool isBetween(float value, float minValue, float maxValue)
{
    return (value >= minValue) && (value <= maxValue);
}

static Direction selectShiftDirection(float leftShift, float rightShift,
                                      float topShift, float bottomShift,
                                      float minShift, float maxShift)
{
    Direction result = Direction::NONE;
    float bestShift = FIELD_WIDTH * BLOCK_SIZE;
    if (isBetween(leftShift, minShift, maxShift) && (leftShift < bestShift))
    {
        result = Direction::LEFT;
        bestShift = leftShift;
    }
    if (isBetween(rightShift, minShift, maxShift) && (rightShift < bestShift))
    {
        result = Direction::RIGHT;
        bestShift = rightShift;
    }
    if (isBetween(topShift, minShift, maxShift) && (topShift < bestShift))
    {
        result = Direction::UP;
        bestShift = topShift;
    }
    if (isBetween(bottomShift, minShift, maxShift) && bottomShift < bestShift)
    {
        result = Direction::DOWN;
        bestShift = bottomShift;
    }

    return result;
}

// Находит символ `marker` в исходной карте лабиринта.
static sf::Vector2f getStartPosition(char marker)
{
    for (size_t y = 0; y < FIELD_HEIGHT; y++)
    {
        for (size_t x = 0; x < FIELD_WIDTH; x++)
        {
            const size_t offset = x + y * FIELD_WIDTH;
            if (FIELD_MAZE[offset] == marker)
            {
                return { x * BLOCK_SIZE, y * BLOCK_SIZE };
            }
        }
    }
    return { 0, 0 };
}

sf::Vector2f getPackmanStartPosition()
{
    return getStartPosition('@');
}

sf::Vector2f getBlinkyStartPosition()
{
    return getStartPosition('B');
}

sf::Vector2f getPinkyStartPosition()
{
    return getStartPosition('P');
}

sf::Vector2f getInkyStartPosition()
{
    return getStartPosition('I');
}

sf::Vector2f getClydeStartPosition()
{
    return getStartPosition('C');
}

void initializeField(Field &field)
{
    field.width = FIELD_WIDTH;
    field.height = FIELD_HEIGHT;
    field.cells = new Cell[field.width * field.height];
    for (size_t y = 0; y < field.height; y++)
    {
        for (size_t x = 0; x < field.width; x++)
        {
            const size_t offset = x + y * field.width;
            CellCategory category;
            sf::Color color;
            if (FIELD_MAZE[offset] == '#')
            {
                category = CellCategory::WALL;
                color = BROWN_COLOR;
            }
            else
            {
                category = CellCategory::ROAD;
                color = WHITE_COLOR;
            }
            Cell &cell = field.cells[offset];
            cell.category = category;
            cell.bounds.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
            cell.bounds.setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
            cell.bounds.setFillColor(color);
        }
    }
}

void drawField(sf::RenderWindow &window, const Field &field)
{
    for (size_t i = 0; i < field.width * field.height; i++)
    {
        window.draw(field.cells[i].bounds);
    }
}

// Модифицирует вектор перемещения, избегая столкновения
// прямоугольника `rect` со стенами лабиринта в поле `field`.
// Возвращает `true`, если вектор перемещения изменён.
bool checkFieldWallsCollision(const Field &field, const sf::FloatRect &oldBounds, sf::Vector2f &movement)
{
    sf::FloatRect newBounds = moveRect(oldBounds, movement);
    bool changed = false;
    for (size_t i = 0, n = field.width * field.height; i < n; i++)
    {
        const Cell &cell = field.cells[i];
        if (cell.category == CellCategory::ROAD)
        {
            continue;
        }

        sf::FloatRect blockBound = cell.bounds.getGlobalBounds();
        if (newBounds.intersects(blockBound))
        {
            const float bottomShift = getBottom(blockBound) - newBounds.top;
            const float topShift = getBottom(newBounds) - blockBound.top;
            const float rightShift = getRight(blockBound) - newBounds.left;
            const float leftShift = getRight(newBounds) - blockBound.left;
            const float movementShift = std::max(std::abs(movement.x), std::abs(movement.y));

            Direction direction = selectShiftDirection(leftShift, rightShift,
                                                       topShift, bottomShift,
                                                       movementShift + EPSILON, MAX_SHIFT);
            if (direction == Direction::NONE)
            {
                direction = selectShiftDirection(leftShift, rightShift,
                                                 topShift, bottomShift,
                                                 0, MAX_SHIFT);
            }
            switch (direction)
            {
            case Direction::UP:
                movement.y -= topShift;
                break;
            case Direction::DOWN:
                movement.y += bottomShift;
                break;
            case Direction::LEFT:
                movement.x -= leftShift;
                break;
            case Direction::RIGHT:
                movement.x += rightShift;
                break;
            case Direction::NONE:
                break;
            }
            changed = true;
            newBounds = moveRect(oldBounds, movement);
        }
    }
    return changed;
}

void destroyField(Field &field)
{
    delete[] field.cells;
}
