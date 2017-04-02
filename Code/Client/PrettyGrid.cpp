#include <Game/Constants.h>
#include <Game/Vector2i.h>
#include <Game/BikeState.h>
#include <Game/Simulation.h>
#include "PrettyGrid.h"
#include "AssetManager.h"

PrettyGrid::PrettyGrid(AssetManager* _asset_manager)
    : asset_manager(_asset_manager)
{
    initGrid();
    initPlayerMarkers();
}

void PrettyGrid::tick(double _dt)
{
    for (auto& marker : player_markers)
    {
        marker.tick(_dt);
    }
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

    for (auto& marker : player_markers)
    {
        marker.draw(_window);
    }
}

void PrettyGrid::clearCell(const Vector2i& _pos)
{
    setTileColor(_pos, sf::Color::Transparent);
}

void PrettyGrid::clearCellRange(const std::vector<Vector2i>& _positions)
{
    for (auto& pos : _positions)
    {
        clearCell(pos);
    }
}

void PrettyGrid::clearAllCells()
{
    for (auto& tile : tiles)
    {
        tile->setFillColor(sf::Color::Transparent);
    }
}

void PrettyGrid::overwriteCell(const Vector2i& _pos, const CellValue _value)
{
    setTileColor(_pos, evaluateSFColor(_value));
}

void PrettyGrid::overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value)
{
    for (auto& pos : _positions)
    {
        setTileColor(pos, evaluateSFColor(_value));
    }
}

void PrettyGrid::overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells)
{
    for (int i = 0; i < GRID_AREA; ++i)
    {
        CellValue value = _cells[i];
        overwriteCell({ i % GRID_SIZE_X, i / GRID_SIZE_X }, value);
    }
}

void PrettyGrid::updatePlayerMarker(const BikeState& _bike_state)
{
    auto& marker = player_markers[_bike_state.id];

    if (!_bike_state.alive)
    {
        marker.setVisible(false);
        return;
    }

    marker.setVisible(true);
    marker.setEnlarged(_bike_state.boosting);
    marker.setPosition(tiles[calculateIndex(_bike_state.pos)]->getPosition());
}

void PrettyGrid::removeAllPlayerMarkers()
{
    for (auto& marker : player_markers)
    {
        marker.setVisible(false);
    }
}

void PrettyGrid::updateBikePosition(const Vector2i& _pos, const unsigned int _bike_id)
{
    player_markers[_bike_id].setPosition(tiles[calculateIndex(_pos)]->getPosition());
    setTileColor(_pos, sf::Color::White);
}

void PrettyGrid::initGrid()
{
    float pane_width = WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY;
    float pane_height = WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY;
    sf::Vector2f pane({ pane_width, pane_height });

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
            auto tile = std::make_unique<sf::RectangleShape>(rect);

            tile->setFillColor(sf::Color::Transparent);

            tile->setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * rect.x), 
                                     WINDOW_TOP_BOUNDARY + (y_cycles * rect.y) });

            tiles[calculateIndex(x_cycles, y_cycles)] = std::move(tile);
        }
    }
}

void PrettyGrid::initPlayerMarkers()
{
    auto* tex = asset_manager->loadTexture(PLAYER_MARKER);

    sf::Sprite sprite(*tex);
    sprite.setPosition({ -100, -100 });
    sprite.setOrigin({ 32, 32 });
    sprite.setScale({ 0.75f, 0.75f });

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        sprite.setColor(evaluateSFColor(Simulation::idToCellValue(i)));
        player_markers[i].setSprite(sprite);
    }
}

void PrettyGrid::setTileColor(const unsigned int _index, const sf::Color& _color)
{
    tiles[_index]->setFillColor(_color);
}

void PrettyGrid::setTileColor(const Vector2i& _pos, const sf::Color& _color)
{
    setTileColor(calculateIndex(_pos.x, _pos.y), _color);
}

sf::Color PrettyGrid::evaluateSFColor(const CellValue _value) const
{
    switch (_value)
    {
        case CellValue::CYAN: return sf::Color::Cyan;
        case CellValue::GREEN: return sf::Color::Green;
        case CellValue::MAGENTA: return sf::Color::Magenta;
        case CellValue::YELLOW: return sf::Color::Yellow;

        default: return sf::Color::Transparent;
    }
}

int PrettyGrid::calculateIndex(const int _x, const int _y) const
{
    return (_y * GRID_SIZE_X) + _x;
}

int PrettyGrid::calculateIndex(const Vector2i& _pos) const
{
    return calculateIndex(_pos.x, _pos.y);
}
