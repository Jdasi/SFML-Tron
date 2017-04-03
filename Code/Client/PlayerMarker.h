#pragma once
#include <SFML/Graphics.hpp>

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
    void setVisible(const bool _value);

    void setEnlarged(const bool _value);

private:
    sf::Sprite sprite;
    bool visible;
    bool enlarged;

};
