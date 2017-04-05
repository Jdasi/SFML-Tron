#pragma once
#include <vector>
#include <array>
#include <memory>

#include <SFML/Graphics.hpp>

#include <Game/SimulationListener.h>
#include "PlayerMarker.h"

struct Vector2i;
struct ClientData;

/* Class for visualising the current state of the simulation on the client.
 * The Visualisation must be attached to the simulation to function properly.
 *
 * The Visualisation listens to events from the simulation and
 * updates its visuals accordingly.
 */
class Visualisation final : public SimulationListener
{
public:
    explicit Visualisation(ClientData* _client_data);
    ~Visualisation() = default;

    void tick(const double _dt);
    void draw(sf::RenderWindow& _window);

    void updateClientColor();

private:
    void initGrid();
    void initPlayerMarkers();
    void initBoostChargesText();
    void updateBoostChargesText();

    void setTileColor(const unsigned int _index, const sf::Color& _color);
    void setTileColor(const Vector2i& _pos, const sf::Color& _color);

    // SimulationListener events.
    void clearCell(const Vector2i& _pos) override;
    void clearCellRange(const std::vector<Vector2i>& _positions) override;
    void simulationReset() override;

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
    void boostChargeGranted(const unsigned int _bike_id) override;

    ClientData* client_data;
    sf::Sprite backdrop;
    sf::RectangleShape border;
    std::array<std::unique_ptr<sf::RectangleShape>, GRID_AREA> tiles;
    std::array<PlayerMarker, MAX_PLAYERS> player_markers;
    sf::Text boost_charges_display;
    unsigned int boost_charges;

};
