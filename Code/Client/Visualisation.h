#pragma once
#include <vector>
#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>
#include "PlayerMarker.h"

struct Vector2i;
class AssetManager;

class Visualisation final : public SimulationListener
{
public:
    explicit Visualisation(AssetManager* _asset_manager);
    ~Visualisation() = default;

    void tick(const double _dt);
    void draw(sf::RenderWindow& _window);

    void updateBorderColor(const sf::Color& _color);

    void clearCell(const Vector2i& _pos) override;
    void clearCellRange(const std::vector<Vector2i>& _positions) override;
    void clearAllCells() override;

    void overwriteCell(const Vector2i& _pos, const CellValue _value) override;
    void overwriteCellRange(const std::vector<Vector2i>& _positions, 
        const CellValue _value) override;
    void overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells) override;

    void updateBikePosition(const unsigned int _bike_id, const Vector2i& _bike_pos,
        const bool _bike_alive) override;
    void bikeBoosted(const unsigned int _bike_id) override;
    void bikeNotBoosted(const unsigned int _bike_id) override;
    void bikeRemoved(const unsigned int _bike_id) override;
    void bikesReset() override;

private:
    void initGrid();
    void initPlayerMarkers();

    void setTileColor(const unsigned int _index, const sf::Color& _color);
    void setTileColor(const Vector2i& _pos, const sf::Color& _color);

    AssetManager* asset_manager;

    sf::Sprite backdrop;
    sf::RectangleShape border;
    std::array<std::unique_ptr<sf::RectangleShape>, GRID_AREA> tiles;
    std::array<PlayerMarker, MAX_PLAYERS> player_markers;

};
