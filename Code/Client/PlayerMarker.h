#pragma once
#include <SFML/Graphics.hpp>

/* Class that contains an sf::Sprite which continuously rotates
 * when it is ticked.
 *
 * Used by Visualisation to mark the current position of a bike
 * within the simulation.
 */
class PlayerMarker
{
public:
    PlayerMarker();
    ~PlayerMarker() = default;

    void tick(const double _dt);
    void draw(sf::RenderWindow& _window) const;

    void setSprite(const sf::Sprite& _sprite);
    void setPosition(const sf::Vector2f& _pos);

    bool isVisible() const;
    void setVisible(const bool _visible);

    void setEnlarged(const bool _enlarged);

private:
    sf::Sprite sprite;
    bool visible;
    bool enlarged;

};
