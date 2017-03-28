#include <Game/Constants.h>
#include <Game/Vector2i.h>
#include <Game/CellValue.h>
#include <Game/Cell.h>
#include "PrettyGrid.h"

PrettyGrid::PrettyGrid()
{
    tiles.reserve(GRID_SIZE_X * GRID_SIZE_Y);
    initGrid();
}

void PrettyGrid::draw(sf::RenderWindow& _window)
{
    _window.draw(border);

    for (auto& tile : tiles)
    {
        if (tile->getFillColor() == sf::Color::Transparent)
        {
            continue;
        }

        _window.draw(*tile);
    }
}

void PrettyGrid::overwriteCell(const Vector2i& _pos,
    const CellValue _value, const CellColour _colour)
{
    sf::Color color;

    switch (_value)
    {
        case CellValue::NONE: color = sf::Color::Transparent; break;
        case CellValue::HEAD: color = sf::Color::White; break;
        case CellValue::TRAIL: color = evaluateSFColor(_colour);

    default: {}
    }

    setTileColor(_pos, color);
}

void PrettyGrid::overwriteCellRange(const std::vector<Vector2i>& _positions,
    const CellValue _value, const CellColour _colour)
{
    for (auto& pos : _positions)
    {
        overwriteCell(pos, _value, _colour);
    }
}

void PrettyGrid::overwriteAllCells(const std::array<Cell, GRID_AREA>& _cells)
{
    for (int i = 0; i < GRID_AREA; ++i)
    {
        Cell cell = _cells[i];
        overwriteCell({ i % GRID_SIZE_X, i / GRID_SIZE_X }, cell.value, cell.colour);
    }
}

void PrettyGrid::initGrid()
{
    float pane_width = WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY;
    float pane_height = WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY;
    sf::Vector2f pane({ pane_width, pane_height });

    border.setOrigin({ 0.5f, 0.5f });
    border.setPosition({ WINDOW_LEFT_BOUNDARY, WINDOW_TOP_BOUNDARY });
    border.setSize(pane);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(5.0f);
    border.setOutlineColor(sf::Color::White);

    float rect_width =  pane_width / GRID_SIZE_X;
    float rect_height =  pane_height / GRID_SIZE_Y;
    sf::Vector2f rect({ rect_width, rect_height });

    for (int y_cycles = 0; y_cycles < GRID_SIZE_Y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < GRID_SIZE_X; ++x_cycles)
        {
            auto rectangle = std::make_unique<sf::RectangleShape>(rect);
            rectangle->setOrigin({ 0.5f, 0.5f });

            rectangle->setFillColor(sf::Color::Transparent);

            rectangle->setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * rect.x), 
                                     WINDOW_TOP_BOUNDARY + (y_cycles * rect.y) });

            tiles.push_back(std::move(rectangle));
        }
    }
}

void PrettyGrid::setTileColor(int _index, sf::Color _color)
{
    tiles[_index]->setFillColor(_color);
}

void PrettyGrid::setTileColor(const Vector2i _pos, sf::Color _color)
{
    setTileColor(calculateTilesIndex(_pos.x, _pos.y), _color);
}

sf::Color PrettyGrid::evaluateSFColor(CellColour _colour) const
{
    switch (_colour)
    {
        case CellColour::CYAN: return sf::Color::Cyan;
        case CellColour::GREEN: return sf::Color::Green;
        case CellColour::MAGENTA: return sf::Color::Magenta;
        case CellColour::YELLOW: return sf::Color::Yellow;

        default: return sf::Color::Transparent;
    }
}

int PrettyGrid::calculateTilesIndex(int _x, int _y) const
{
    return (_y * GRID_SIZE_X) + _x;
}

int PrettyGrid::calculateTilesIndex(const Vector2i& _pos) const
{
    return calculateTilesIndex(_pos.x, _pos.y);
}
