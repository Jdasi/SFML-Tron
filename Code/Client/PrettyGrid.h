#pragma once
#include <vector>
#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>
#include "PlayerMarker.h"

struct Vector2i;
class AssetManager;

class PrettyGrid : public SimulationListener
{
public:
    PrettyGrid(AssetManager* _asset_manager);
    ~PrettyGrid() = default;

    void tick(double _dt);
    void draw(sf::RenderWindow& _window);

    void clearCell(const Vector2i& _pos) override;
    void clearCellRange(const std::vector<Vector2i>& _positions) override;
    void clearAllCells() override;

    void overwriteCell(const Vector2i& _pos, const CellValue _value) override;
    void overwriteCellRange(const std::vector<Vector2i>& _positions, 
        const CellValue _value) override;
    void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells) override;

    void addPlayerMarker(const unsigned int _bike_id, const CellValue _value) override;
    void updatePlayerMarkerSize(const unsigned int _bike_id, const bool _enlarged) override;

    void removePlayerMarker(const unsigned int _bike_id) override;
    void removeAllPlayerMarkers() override;
    void updateBikePosition(const Vector2i& _pos, const unsigned int _bike_id) override;

private:
    void initGrid();
    void setTileColor(const unsigned int _index, const sf::Color& _color);
    void setTileColor(const Vector2i& _pos, const sf::Color& _color);
    sf::Color evaluateSFColor(const CellValue _value) const;

    int calculateIndex(const int _x, const int _y) const;
    int calculateIndex(const Vector2i& _pos) const;

    AssetManager* asset_manager;

    sf::RectangleShape border;
    std::array<std::unique_ptr<sf::RectangleShape>, GRID_AREA> tiles;
    std::array<PlayerMarker, MAX_PLAYERS> player_markers;

};
