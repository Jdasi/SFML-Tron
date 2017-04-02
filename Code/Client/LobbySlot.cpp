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
}

void LobbySlot::setPlayerIDText(const std::string& _str)
{
    player_id_text.setString(_str);
    centerText(player_id_text);
}

void LobbySlot::setPlayerStateText(const std::string& _str)
{
    player_state_text.setString(_str);
    centerText(player_state_text);
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
    player_id_text.setPosition(_pos + sf::Vector2f{ 0, -20 });
    player_state_text.setPosition(_pos + sf::Vector2f{ 0, 20 });
}

void LobbySlot::setRectangleSize(const sf::Vector2f& _size)
{
    rectangle.setSize(_size);

    // Center rectangle.
    sf::FloatRect bounds = rectangle.getLocalBounds();
    rectangle.setOrigin(bounds.left + bounds.width / 2.0f, 
        bounds.top + bounds.height / 2.0f);
}

void LobbySlot::draw(sf::RenderWindow& _window) const
{
    _window.draw(rectangle);
    _window.draw(player_id_text);
    _window.draw(player_state_text);
}

void LobbySlot::centerText(sf::Text& _text) const
{
    sf::FloatRect bounds = _text.getLocalBounds();
    _text.setOrigin(bounds.left + bounds.width / 2.0f, 
        bounds.top + bounds.height / 2.0f);
}
