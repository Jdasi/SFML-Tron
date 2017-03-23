#include "GameGrid.h"
#include <Game/Constants.h>

GameGrid::GameGrid(int _size_x, int _size_y)
    : size_x(_size_x)
    , size_y(_size_y)
    , area(_size_x * _size_y)
{
    tiles.reserve(area);
    initGrid();
}

void GameGrid::draw(sf::RenderWindow& _window)
{
    for (auto& tile : tiles)
    {
        _window.draw(*tile);
    }
}

sf::Color GameGrid::getTileColor(sf::Vector2<int> _pos)
{
    return tiles[calculateTilesIndex(_pos.x, _pos.y)]->getFillColor();
}

void GameGrid::setTileColor(sf::Vector2<int> _pos, sf::Color _color)
{
    tiles[calculateTilesIndex(_pos.x, _pos.y)]->setFillColor(_color);
}

void GameGrid::initGrid()
{
    sf::Vector2f rect({ static_cast<float>(WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY) / size_x, 
                        static_cast<float>(WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY) / size_y });

    for (int y_cycles = 0; y_cycles < size_y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < size_x; ++x_cycles)
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

int GameGrid::calculateTilesIndex(int _x, int _y) const
{
    return (_y * size_x) + _x;
}
