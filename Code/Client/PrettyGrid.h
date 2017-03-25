#pragma once
#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>
#include <vector>
#include <memory>

struct Vector2i;
enum CellColour;

class PrettyGrid : public SimulationListener
{
public:
    PrettyGrid();
    ~PrettyGrid() = default;

    void draw(sf::RenderWindow& _window);
    void updateCell(const Bike& _bike, CellValue _value) override;
    void updateAllCells(std::vector<Cell> _cells) override;

private:
    void initGrid();
    void setTileColor(int _index, sf::Color _color);
    void setTileColor(const Vector2i _pos, sf::Color _color);
    sf::Color evaluateSFColor(CellColour _colour) const;

    int calculateTilesIndex(int _x, int _y) const;

    sf::RectangleShape border;
    std::vector<std::unique_ptr<sf::RectangleShape>> tiles;

};
