#pragma once
#include <SFML/Graphics.hpp>

/* Class for displaying a rectangle shape with two text objects.
 * Used by LobbyUI for visualising the list of players on the server.
 */
class LobbySlot final
{
public:
    explicit LobbySlot(sf::Font* _font);
    ~LobbySlot() = default;

    void setPlayerIDText(const std::string& _str);
    void setPlayerStateText(const std::string& _str);

    void setOccupied(const bool _occupied);
    void setOccupiedColor(const sf::Color& _color);

    void setPosition(const sf::Vector2f& _pos);
    void setRectangleSize(const sf::Vector2f& _size);
    void draw(sf::RenderWindow& _window) const;

private:
    sf::Font* font;

    sf::RectangleShape rectangle;
    sf::Text player_id_text;
    sf::Text player_state_text;

    bool occupied;
    sf::Color occupied_color;
    sf::Color vacant_color;

};
