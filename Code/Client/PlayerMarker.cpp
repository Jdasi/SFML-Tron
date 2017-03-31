#include <Game/Constants.h>
#include "PlayerMarker.h"

PlayerMarker::PlayerMarker(const sf::Sprite& _sprite)
    : sprite(_sprite)
    , enlarged(false)
{
}

void PlayerMarker::tick(double _dt)
{
    float modifier = enlarged ? MARKER_ROTATE_MODIFIER : 1.0f;
    sprite.rotate(static_cast<float>(MARKER_ROTATE_SPEED * modifier * _dt));
}

void PlayerMarker::draw(sf::RenderWindow& _window) const
{
    _window.draw(sprite);
}

void PlayerMarker::setPosition(const sf::Vector2f& _pos)
{
    sprite.setPosition(_pos);
}

void PlayerMarker::enlarge(const bool _value)
{
    enlarged = _value;

    if (enlarged)
    {
        sprite.setScale({ 1.0f, 1.0f });
    }
    else
    {
        sprite.setScale({ 0.75f, 0.75f });
    }

}
