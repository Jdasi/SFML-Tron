#pragma once
#include <SFML/Graphics.hpp>

class PlayerMarker
{
public:
    PlayerMarker(const sf::Sprite& _sprite);
    ~PlayerMarker() = default;

    void tick(double _dt);
    void draw(sf::RenderWindow& _window) const;

    void setPosition(const sf::Vector2f& _pos);

    void enlarge(const bool _value);

private:
    sf::Sprite sprite;
    bool enlarged;

};
