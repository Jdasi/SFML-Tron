#include <Game/Constants.h>
#include <Game/Vector2i.h>
#include <Game/JHelper.h>
#include "Visualisation.h"
#include "ClientData.h"
#include "AssetManager.h"
#include "GameAudio.h"

Visualisation::Visualisation(ClientData* _client_data)
    : client_data(_client_data)
    , boost_charges(STARTING_BOOST_CHARGES)
{
    auto& asset_manager = client_data->asset_manager;

    backdrop.setTexture(*asset_manager->loadTexture(BACKDROP));
    boost_charges_display.setFont(*asset_manager->loadFont(DEFAULT_FONT));

    initGrid();
    initPlayerMarkers();
    initBoostChargesText();
}



// Animates all of the rotating player markers.
void Visualisation::tick(const double _dt)
{
    for (auto& marker : player_markers)
    {
        marker.tick(_dt);
    }
}



void Visualisation::draw(sf::RenderWindow& _window)
{
    _window.draw(backdrop);
    _window.draw(border);
    _window.draw(boost_charges_display);

    for (auto& tile : tiles)
    {
        if (tile.getFillColor() == sf::Color::Transparent)
        {
            continue;
        }

        _window.draw(tile);
    }

    for (auto& marker : player_markers)
    {
        marker.draw(_window);
    }
}



void Visualisation::updateClientColor()
{
    sf::Color color = JHelper::evaluateSFColorFromPlayerID(client_data->client_id);

    border.setOutlineColor(color);
    boost_charges_display.setFillColor(color);
}



/* Configures the layout of the grid based on the settings
 * in the Constants.h file.
 */
void Visualisation::initGrid()
{
    float pane_width = WINDOW_RIGHT_BOUNDARY - WINDOW_LEFT_BOUNDARY;
    float pane_height = WINDOW_BOTTOM_BOUNDARY - WINDOW_TOP_BOUNDARY;
    sf::Vector2f pane({ pane_width, pane_height });

    border.setPosition({ WINDOW_LEFT_BOUNDARY, WINDOW_TOP_BOUNDARY });
    border.setSize(pane);
    border.setFillColor(sf::Color::Black);
    border.setOutlineThickness(5.0f);

    float rect_width =  pane_width / GRID_SIZE_X;
    float rect_height =  pane_height / GRID_SIZE_Y;
    sf::Vector2f rect({ rect_width, rect_height });

    for (int y_cycles = 0; y_cycles < GRID_SIZE_Y; ++y_cycles)
    {
        for (int x_cycles = 0; x_cycles < GRID_SIZE_X; ++x_cycles)
        {
            auto& tile = tiles[JHelper::calculateIndex(x_cycles, y_cycles, GRID_SIZE_X)];

            tile.setSize(rect);
            tile.setFillColor(sf::Color::Transparent);
            tile.setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * rect.x), 
                                     WINDOW_TOP_BOUNDARY + (y_cycles * rect.y) });

        }
    }
}



/* Configures all player markers ready for the simulation.
 * All markers are hidden to start, and they reveal themselves as
 * the simulation becomes populated with bikes.
 */
void Visualisation::initPlayerMarkers()
{
    auto* tex = client_data->asset_manager->loadTexture(PLAYER_MARKER);

    sf::Sprite sprite(*tex);
    sprite.setPosition({ -100, -100 });

    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        sprite.setColor(JHelper::evaluateSFColorFromPlayerID(i));
        auto& marker = player_markers[i];

        marker.setSprite(sprite);
        marker.setEnlarged(false);
    }
}



void Visualisation::initBoostChargesText()
{
    boost_charges_display.setCharacterSize(30);
    boost_charges_display.setStyle(sf::Text::Bold);
    boost_charges_display.setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.05f });
    boost_charges_display.setOutlineColor(sf::Color::Black);
    boost_charges_display.setOutlineThickness(2.0f);

    updateBoostChargesText();
}



/* Updates and centers the boost charges text.
 * This function should be called whenever the boost_charges variable changes.
 */
void Visualisation::updateBoostChargesText()
{
    std::string str("Boost Charges: ");
    str.append(std::to_string(boost_charges));

    boost_charges_display.setString(str);

    JHelper::centerSFOrigin(boost_charges_display);
}



void Visualisation::setTileColor(const unsigned int _index, const sf::Color& _color)
{
    tiles[_index].setFillColor(_color);
}



void Visualisation::setTileColor(const Vector2i& _pos, const sf::Color& _color)
{
    setTileColor(JHelper::calculateIndex(_pos.x, _pos.y, GRID_SIZE_X), _color);
}



// Called when the simulation clears a cell.
void Visualisation::clearCell(const Vector2i& _pos)
{
    setTileColor(_pos, sf::Color::Transparent);
}



// Called when the simulation clears a range of cells.
void Visualisation::clearCellRange(const std::vector<Vector2i>& _positions)
{
    for (auto& pos : _positions)
    {
        clearCell(pos);
    }
}



// Called when the simulation resets.
void Visualisation::simulationReset()
{
    for (auto& tile : tiles)
    {
        tile.setFillColor(sf::Color::Transparent);
    }

    boost_charges = STARTING_BOOST_CHARGES;
    updateBoostChargesText();
}



// Called when a cell in the simulation changes.
void Visualisation::overwriteCell(const Vector2i& _pos, const CellValue _value)
{
    setTileColor(_pos, JHelper::evaluateSFColor(_value));
}



// Called when a range of cells in the simulation change.
void Visualisation::overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value)
{
    for (auto& pos : _positions)
    {
        setTileColor(pos, JHelper::evaluateSFColor(_value));
    }
}



// Called when all cells in the simulation change
void Visualisation::overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells)
{
    for (int i = 0; i < GRID_AREA; ++i)
    {
        CellValue value = _cells[i];
        overwriteCell({ i % GRID_SIZE_X, i / GRID_SIZE_X }, value);
    }
}



// Called when the position of a bike in the simulation changes.
void Visualisation::updateBikePosition(const unsigned int _bike_id, const Vector2i& _bike_pos,
    const bool _bike_alive)
{
    auto& marker = player_markers[_bike_id];

    if (!_bike_alive)
    {
        marker.setVisible(false);
        return;
    }

    marker.setVisible(true);
    marker.setPosition(
        tiles[JHelper::calculateIndex(_bike_pos, GRID_SIZE_X)].getPosition());

    setTileColor(_bike_pos, sf::Color::White);
}



// Called when a bike first activates its boost.
void Visualisation::bikeBoosted(const unsigned int _bike_id)
{
    client_data->game_audio->playSound(BOOST_CUE);

    player_markers[_bike_id].setEnlarged(true);

    if (_bike_id == client_data->client_id)
    {
        --boost_charges;
        updateBoostChargesText();
    }
}



// Called when a bike is first found to no longer be boosting.
void Visualisation::bikeNotBoosted(const unsigned int _bike_id)
{
    player_markers[_bike_id].setEnlarged(false);
}



// Called when a bike is destroyed or the controlling player leaves.
void Visualisation::bikeRemoved(const unsigned int _bike_id)
{
    client_data->game_audio->playSound(DEATH_CUE);

    player_markers[_bike_id].setVisible(false);
}



// Called when all bikes are removed from the simulation.
void Visualisation::bikesReset()
{
    for (auto& marker : player_markers)
    {
        marker.setVisible(false);
        marker.setEnlarged(false);
    }
}



// Called when a bike receives an additional boost charge.
void Visualisation::boostChargeGranted(const unsigned int _bike_id)
{
    if (_bike_id != client_data->client_id)
    {
        return;
    }

    client_data->game_audio->playSound(EXTRA_BOOST_CUE);

    ++boost_charges;
    updateBoostChargesText();
}
