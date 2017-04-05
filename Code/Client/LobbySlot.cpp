#include <Game/JHelper.h>
#include "LobbySlot.h"

LobbySlot::LobbySlot(sf::Font* _font)
    : font(_font)
    , occupied(false)
    , vacant_color(sf::Color(100, 100, 100, 255))
{
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineColor(vacant_color);
    rectangle.setOutlineThickness(5.0f);

    player_id_text.setFont(*font);
    player_state_text.setFont(*font);

    player_state_text.setCharacterSize(20);
    player_state_text.setFillColor(sf::Color(150, 150, 150, 255));
}



void LobbySlot::setPlayerIDText(const std::string& _str)
{
    player_id_text.setString(_str);
    JHelper::centerSFOrigin(player_id_text);
}



void LobbySlot::setPlayerStateText(const std::string& _str)
{
    player_state_text.setString(_str);
    JHelper::centerSFOrigin(player_state_text);
}



/* Updates the slot's occupied status. 
 * The border and text objects are updated automatically.
 */
void LobbySlot::setOccupied(const bool _occupied)
{
    occupied = _occupied;

    rectangle.setOutlineColor(occupied ? occupied_color : vacant_color);

    if (!occupied)
    {
        setPlayerStateText("Empty");
    }
}



// Informs the slot of its border colour when the slot is marked as occupied.
void LobbySlot::setOccupiedColor(const sf::Color& _color)
{
    occupied_color = _color;
}



/* Sets overall position of the slot. 
 * The text objects are automatically arranged based on the new position.
 */
void LobbySlot::setPosition(const sf::Vector2f& _pos)
{
    rectangle.setPosition(_pos);
    player_id_text.setPosition(_pos + sf::Vector2f{ 0, -10 });
    player_state_text.setPosition(_pos + sf::Vector2f{ 0, 20 });
}



// Updates the size and origin of the rectangle. The text objects are unaffected.
void LobbySlot::setRectangleSize(const sf::Vector2f& _size)
{
    rectangle.setSize(_size);

    JHelper::centerSFOrigin(rectangle);
}



void LobbySlot::draw(sf::RenderWindow& _window) const
{
    _window.draw(rectangle);
    _window.draw(player_id_text);
    _window.draw(player_state_text);
}
