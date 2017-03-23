#pragma once
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

class GameGrid
{
public:
    GameGrid(int _size_x, int _size_y);
    ~GameGrid() = default;

    void draw(sf::RenderWindow& _window);

    sf::Color getTileColor(sf::Vector2<int> _pos);
    void setTileColor(sf::Vector2<int> _pos, sf::Color _color);

private:
    void initGrid();
    int calculateTilesIndex(int _x, int _y) const;

    int size_x;
    int size_y;
    int area;

    sf::RectangleShape border;
    std::vector<std::unique_ptr<sf::RectangleShape>> tiles;

};
