#include "field.h"
#include "packman.h"
#include <cmath>
#include <limits>
#include <cassert>

static const float BLOCK_SIZE = 32.f;
static const float EPSILON = std::numeric_limits<float>::epsilon();
static const float MAX_SHIFT = 0.5f * BLOCK_SIZE;
static const float MIN_COOKIE_OVERLAP_AREA = 400.f;

static const float COOKIE_RADIUS = 2.f;
static const float SUPERCOOKIE_RADIUS = 5.f;

static const char WALL_MARKER = '#';
static const char UNREACHABLE_MARKER = '!';
static const char COOKIE_MARKER = ' ';
static const char SUPERCOOKIE_MARKER = '$';

// Полноценная версия игрового поля.
#if 1

static const size_t FIELD_WIDTH = 25;
static const size_t FIELD_HEIGHT = 25;
static const char FIELD_MAZE[] =
        "!#######################!"
        "!#          #          #!"
        "!# ## ##### # ##### ## #!"
        "!#                     #!"
        "!# ## # ######### # ## #!"
        "!#    #     #     #    #!"
        "!#### ##### # ##### ####!"
        "!!!!# #     C     # #!!!!"
        "##### # # ##### # # #####"
        "#       # #BPI# #       #"
        "##### # # ##### # # #####"
        "!!!!# #           # #!!!!"
        "!#### # ######### # ####!"
        "!#          #          #!"
        "!# ## ##### # ##### ## #!"
        "!#  #           @   #  #!"
        "!## # # ######### # #  #!"
        "!# $  #     #     #    #!"
        "!# ####### ### ####### #!"
        "!# #     #     #     # #!"
        "!# # ### ## # ## ### # #!"
        "!# # #      #      # # #!"
        "!# # # #### # #### # # #!"
        "!#          #          #!"
        "!#######################!";

// Отладочная версия игрового поля.
#else

static const size_t FIELD_WIDTH = 6;
static const size_t FIELD_HEIGHT = 6;
static const char FIELD_MAZE[] =
        "######"
        "#@ #C#"
        "#  #B#"
        "#$ #P#"
        "#  #I#"
        "######";
#endif

static const sf::Color WALL_COLOR = sf::Color(52, 93, 199);
static const sf::Color ROAD_COLOR = sf::Color(40, 40, 40);
static const sf::Color COOKIE_COLOR = sf::Color(255, 255, 255);

struct FieldGraphics
{
    sf::RectangleShape roadShape;
    sf::RectangleShape wallShape;
    sf::CircleShape cookieShape;
    sf::CircleShape superCookieShape;
};

void initFieldGraphics(FieldGraphics &graphics)
{
    graphics.wallShape.setFillColor(WALL_COLOR);
    graphics.wallShape.setSize({BLOCK_SIZE, BLOCK_SIZE});
    graphics.roadShape.setFillColor(ROAD_COLOR);
    graphics.roadShape.setSize({BLOCK_SIZE, BLOCK_SIZE});
    graphics.cookieShape.setRadius(COOKIE_RADIUS);
    graphics.cookieShape.setFillColor(COOKIE_COLOR);
    graphics.superCookieShape.setRadius(SUPERCOOKIE_RADIUS);
    graphics.superCookieShape.setFillColor(COOKIE_COLOR);
}

static sf::FloatRect moveRect(const sf::FloatRect &rect, sf::Vector2f &offset)
{
    return {rect.left + offset.x, rect.top + offset.y, rect.width, rect.height};
}

static float getArea(const sf::FloatRect &rect)
{
    return rect.width * rect.height;
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

sf::Vector2f getGhostStartPosition(GhostId ghostId)
{
    switch (ghostId)
    {
    case GhostId::BLINKY:
        return getStartPosition('B');
    case GhostId::PINKY:
        return getStartPosition('P');
    case GhostId::INKY:
        return getStartPosition('I');
    case GhostId::CLYDE:
        return getStartPosition('C');
    default:
        assert(false);
    }
}

unsigned countRemainingCookies(const Field &field)
{
    unsigned result = 0;
    for (size_t offset = 0; offset < field.width * field.height; offset++)
    {
        const Cell &cell = field.cells[offset];
        switch (cell.category)
        {
        case CellCategory::COOKIE:
        case CellCategory::SUPERCOOKIE:
            ++result;
            break;
        default:
            break;
        }
    }

    return result;
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
            switch (FIELD_MAZE[offset])
            {
            case UNREACHABLE_MARKER:
                category = CellCategory::EMPTY;
                break;
            case WALL_MARKER:
                category = CellCategory::WALL;
                break;
            case COOKIE_MARKER:
                category = CellCategory::COOKIE;
                break;
            case SUPERCOOKIE_MARKER:
                category = CellCategory::SUPERCOOKIE;
                break;
            default:
                category = CellCategory::EMPTY;
                break;
            }
            Cell &cell = field.cells[offset];
            cell.category = category;
            cell.bounds.left = x * BLOCK_SIZE;
            cell.bounds.top = y * BLOCK_SIZE;
            cell.bounds.width = BLOCK_SIZE;
            cell.bounds.height = BLOCK_SIZE;
        }
    }
}

void drawField(sf::RenderWindow &window, const Field &field)
{
    FieldGraphics graphics;
    initFieldGraphics(graphics);
    for (size_t i = 0; i < field.width * field.height; i++)
    {
        const Cell &cell = field.cells[i];
        const sf::Vector2f position = { cell.bounds.left, cell.bounds.top };
        const sf::Vector2f center = position
                + sf::Vector2f(0.5f * cell.bounds.width, 0.5f * cell.bounds.height);
        switch (cell.category)
        {
        case CellCategory::WALL:
            graphics.wallShape.setPosition(position);
            window.draw(graphics.wallShape);
            break;
        case CellCategory::EMPTY:
            graphics.roadShape.setPosition(position);
            window.draw(graphics.roadShape);
            break;
        case CellCategory::COOKIE:
            graphics.roadShape.setPosition(position);
            graphics.cookieShape.setPosition(center.x - COOKIE_RADIUS, center.y - COOKIE_RADIUS);
            window.draw(graphics.roadShape);
            window.draw(graphics.cookieShape);
            break;
        case CellCategory::SUPERCOOKIE:
            graphics.roadShape.setPosition(position);
            graphics.superCookieShape.setPosition(center.x - SUPERCOOKIE_RADIUS, center.y - SUPERCOOKIE_RADIUS);
            window.draw(graphics.roadShape);
            window.draw(graphics.superCookieShape);
            break;
        }
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
        if (cell.category != CellCategory::WALL)
        {
            continue;
        }

        sf::FloatRect blockBound = cell.bounds;
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

unsigned eatAllCookiesInBounds(Field &field, const sf::FloatRect &bounds)
{
    unsigned cookiesCount = 0;
    for (size_t i = 0, n = field.width * field.height; i < n; i++)
    {
        Cell &cell = field.cells[i];
        if (cell.category != CellCategory::COOKIE
                && cell.category != CellCategory::SUPERCOOKIE)
        {
            continue;
        }
        sf::FloatRect intersection;
        // Нужно не просто пересекаться с печеньем, но и иметь
        // достаточную площадь пересечения.
        if (cell.bounds.intersects(bounds, intersection)
                && (getArea(intersection) >= MIN_COOKIE_OVERLAP_AREA))
        {
            ++cookiesCount;
            cell.category = CellCategory::EMPTY;
        }
    }
    return cookiesCount;
}

void destroyField(Field &field)
{
    delete[] field.cells;
}
