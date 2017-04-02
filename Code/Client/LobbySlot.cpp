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

void LobbySlot::setOccupied(const bool _value)
{
    occupied = _value;

    rectangle.setOutlineColor(occupied ? occupied_color : vacant_color);

    if (!occupied)
    {
        setPlayerStateText("Empty");
    }
}

void LobbySlot::setOccupiedColor(const sf::Color& _color)
{
    occupied_color = _color;
}

void LobbySlot::setPosition(const sf::Vector2f& _pos)
{
    rectangle.setPosition(_pos);
    player_id_text.setPosition(_pos + sf::Vector2f{ 0, -10 });
    player_state_text.setPosition(_pos + sf::Vector2f{ 0, 20 });
}

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
