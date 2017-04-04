#include <Game/Constants.h>
#include <Game/Vector2i.h>
#include <Game/JHelper.h>
#include "Visualisation.h"
#include "ClientData.h"
#include "AssetManager.h"

Visualisation::Visualisation(ClientData* _client_data)
    : client_data(_client_data)
    , displayed_charges(STARTING_BOOST_CHARGES)
{
    auto& asset_manager = client_data->asset_manager;

    backdrop.setTexture(*asset_manager->loadTexture(BACKDROP));
    boost_charges_text.setFont(*asset_manager->loadFont(DEFAULT_FONT));

    initGrid();
    initPlayerMarkers();
    initBoostChargesText();
}



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
    _window.draw(boost_charges_text);

    for (auto& tile : tiles)
    {
        if (tile->getFillColor() == sf::Color::Transparent)
        {
            continue;
        }

        _window.draw(*tile);
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
    boost_charges_text.setFillColor(color);
}



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
            auto tile = std::make_unique<sf::RectangleShape>(rect);

            tile->setFillColor(sf::Color::Transparent);

            tile->setPosition({ WINDOW_LEFT_BOUNDARY + (x_cycles * rect.x), 
                                     WINDOW_TOP_BOUNDARY + (y_cycles * rect.y) });

            tiles[JHelper::calculateIndex(x_cycles, y_cycles, GRID_SIZE_X)] = std::move(tile);
        }
    }
}



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
    boost_charges_text.setCharacterSize(30);
    boost_charges_text.setStyle(sf::Text::Bold);
    boost_charges_text.setPosition({ WINDOW_WIDTH / 2, WINDOW_HEIGHT * 0.05f });
    boost_charges_text.setOutlineColor(sf::Color::Black);
    boost_charges_text.setOutlineThickness(2.0f);

    updateBoostChargesText();
}



void Visualisation::updateBoostChargesText()
{
    std::string str("Boost Charges: ");
    str.append(std::to_string(displayed_charges));

    boost_charges_text.setString(str);

    JHelper::centerSFOrigin(boost_charges_text);
}



void Visualisation::clearCell(const Vector2i& _pos)
{
    setTileColor(_pos, sf::Color::Transparent);
}



void Visualisation::clearCellRange(const std::vector<Vector2i>& _positions)
{
    for (auto& pos : _positions)
    {
        clearCell(pos);
    }
}



void Visualisation::simulationReset()
{
    for (auto& tile : tiles)
    {
        tile->setFillColor(sf::Color::Transparent);
    }

    displayed_charges = STARTING_BOOST_CHARGES;
    updateBoostChargesText();
}



void Visualisation::overwriteCell(const Vector2i& _pos, const CellValue _value)
{
    setTileColor(_pos, JHelper::evaluateSFColor(_value));
}



void Visualisation::overwriteCellRange(const std::vector<Vector2i>& _positions, const CellValue _value)
{
    for (auto& pos : _positions)
    {
        setTileColor(pos, JHelper::evaluateSFColor(_value));
    }
}



void Visualisation::overwriteAllCells(const std::array<CellValue, GRID_AREA>& _cells)
{
    for (int i = 0; i < GRID_AREA; ++i)
    {
        CellValue value = _cells[i];
        overwriteCell({ i % GRID_SIZE_X, i / GRID_SIZE_X }, value);
    }
}



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
        tiles[JHelper::calculateIndex(_bike_pos, GRID_SIZE_X)]->getPosition());

    setTileColor(_bike_pos, sf::Color::White);
}



void Visualisation::bikeBoosted(const unsigned int _bike_id)
{
    player_markers[_bike_id].setEnlarged(true);

    if (_bike_id == client_data->client_id)
    {
        --displayed_charges;
        updateBoostChargesText();
    }
}



void Visualisation::bikeNotBoosted(const unsigned int _bike_id)
{
    player_markers[_bike_id].setEnlarged(false);
}



void Visualisation::bikeRemoved(const unsigned int _bike_id)
{
    player_markers[_bike_id].setVisible(false);
}



void Visualisation::bikesReset()
{
    for (auto& marker : player_markers)
    {
        marker.setVisible(false);
        marker.setEnlarged(false);
    }
}



void Visualisation::boostChargeGranted(const unsigned int _bike_id)
{
    if (_bike_id != client_data->client_id)
    {
        return;
    }

    ++displayed_charges;
    updateBoostChargesText();
}



void Visualisation::setTileColor(const unsigned int _index, const sf::Color& _color)
{
    tiles[_index]->setFillColor(_color);
}



void Visualisation::setTileColor(const Vector2i& _pos, const sf::Color& _color)
{
    setTileColor(JHelper::calculateIndex(_pos.x, _pos.y, GRID_SIZE_X), _color);
}
