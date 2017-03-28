#pragma once
#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>
#include <array>
#include <memory>

struct Vector2i;
enum CellColour;

class PrettyGrid : public SimulationListener
{
public:
    PrettyGrid();
    ~PrettyGrid() = default;

    void draw(sf::RenderWindow& _window);

    void overwriteCell(const Vector2i& _pos, const CellValue _value, 
        const CellColour _colour) override;
    void overwriteCellRange(const std::vector<Vector2i>& _positions, 
        const CellValue _value, const CellColour _colour) override;
    void overwriteAllCells(const std::array<Cell, GRID_AREA>& _cells) override;

private:
    void initGrid();
    void setTileColor(int _index, sf::Color _color);
    void setTileColor(const Vector2i _pos, sf::Color _color);
    sf::Color evaluateSFColor(CellColour _colour) const;

    int calculateTilesIndex(int _x, int _y) const;
    int calculateTilesIndex(const Vector2i& _pos) const;



    sf::RectangleShape border;
    std::vector<std::unique_ptr<sf::RectangleShape>> tiles;

};
