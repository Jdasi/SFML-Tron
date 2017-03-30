#pragma once
#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>

struct Vector2i;
enum CellColour;

class PrettyGrid : public SimulationListener
{
public:
    PrettyGrid();
    ~PrettyGrid() = default;

    void draw(sf::RenderWindow& _window);

    void clearCell(const Vector2i& _pos) override;
    void clearCellRange(const std::vector<Vector2i>& _positions) override;
    void overwriteCell(const Vector2i& _pos, const CellValue _value) override;
    void overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value) override;
    void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells) override;
    void updateBikePosition(const Vector2i& _pos, int _bike_id) override;

private:
    void initGrid();
    void setTileColor(int _index, sf::Color _color);
    void setTileColor(const Vector2i& _pos, sf::Color _color);
    sf::Color evaluateSFColor(CellValue _colour) const;

    int calculateIndex(int _x, int _y) const;
    int calculateIndex(const Vector2i& _pos) const;

    sf::RectangleShape border;
    std::vector<std::unique_ptr<sf::RectangleShape>> tiles;

};
