#include <Game/Constants.h>
#include <Game/JHelper.h>
#include "PlayerMarker.h"

PlayerMarker::PlayerMarker()
    : visible(false)
    , enlarged(false)
{
}



/* Rotates the marker each call if the marker is visible. 
 * The rotation speed is determined by the enlarged bool.
 */
void PlayerMarker::tick(const double _dt)
{
    if (!visible)
    {
        return;
    }

    float modifier = enlarged ? MARKER_ROTATE_MODIFIER : 1.0f;
    sprite.rotate(static_cast<float>(MARKER_ROTATE_SPEED * modifier * _dt));
}



// Draws the marker if it is marked as visible.
void PlayerMarker::draw(sf::RenderWindow& _window) const
{
    if (!visible)
    {
        return;
    }

    _window.draw(sprite);
}



void PlayerMarker::setSprite(const sf::Sprite& _sprite)
{
    sprite = _sprite;
}



void PlayerMarker::setPosition(const sf::Vector2f& _pos)
{
    sprite.setPosition(_pos);
}



bool PlayerMarker::isVisible() const
{
    return visible;
}



void PlayerMarker::setVisible(const bool _visible)
{
    visible = _visible;
}



// Updates the size and origin of the marker based on the passed bool.
void PlayerMarker::setEnlarged(const bool _enlarged)
{
    enlarged = _enlarged;

    if (enlarged)
    {
        sprite.setScale({ 1.0f, 1.0f });
    }
    else
    {
        sprite.setScale({ 0.75f, 0.75f });
    }

    JHelper::centerSFOrigin(sprite);
}
