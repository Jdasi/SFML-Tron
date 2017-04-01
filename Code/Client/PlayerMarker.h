#pragma once
#include <SFML/Graphics.hpp>

class PlayerMarker
{
public:
    PlayerMarker();
    ~PlayerMarker() = default;

    void tick(double _dt);
    void draw(sf::RenderWindow& _window) const;

    void setSprite(const sf::Sprite& _sprite);
    void setVisible(const bool _value);
    void setPosition(const sf::Vector2f& _pos);

    void enlarge(const bool _value);

private:
    sf::Sprite sprite;
    bool visible;
    bool enlarged;

};
