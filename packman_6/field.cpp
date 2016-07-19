#include "field.h"

static const float BLOCK_SIZE = 32.f;
static const float COOKIE_RADIUS = 2.f;
static const float SUPERCOOKIE_RADIUS = 5.f;
static const size_t FIELD_WIDTH = 25;
static const size_t FIELD_HEIGHT = 25;

static const char WALL_MARKER = '#';
static const char COOKIE_MARKER = ' ';
static const char SUPERCOOKIE_MARKER = '$';
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
            switch (FIELD_MAZE[offset])
            {
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

        if (newBounds.intersects(cell.bounds))
        {
            if (movement.y < 0)
            {
                movement.y += cell.bounds.top + cell.bounds.height - newBounds.top;
            }
            else if (movement.y > 0)
            {
                movement.y -= newBounds.top + newBounds.height - cell.bounds.top;
            }
            if (movement.x < 0)
            {
                movement.x += cell.bounds.left + cell.bounds.width - newBounds.left;
            }
            else if (movement.x > 0)
            {
                movement.x -= newBounds.left + newBounds.width - cell.bounds.left;
            }
            changed = true;
            newBounds = moveRect(oldBounds, movement);
            break;
        }
    }
    return changed;
}

void destroyField(Field &field)
{
    delete[] field.cells;
}
