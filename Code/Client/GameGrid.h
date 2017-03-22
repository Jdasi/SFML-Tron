#pragma once
#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

class GameGrid
{
public:
    GameGrid(int _size_x, int _size_y);
    ~GameGrid() = default;

private:
    int size_x;
    int size_y;

    std::vector<std::unique_ptr<sf::RectangleShape>> tiles;

};
