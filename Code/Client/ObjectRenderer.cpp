#include <SFML/Graphics.hpp>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(sf::RenderWindow& _window)
    : window(_window)
    , drawables(nullptr)
{
}

void ObjectRenderer::draw() const
{
    // No drawables linked.
    if (!drawables)
    {
        return;
    }

    window.clear();
    for (auto& drawable : *drawables)
    {
        window.draw(*drawable);
    }
    window.display();
}

void ObjectRenderer::link(std::vector<std::unique_ptr<sf::Drawable>>* _drawables)
{
    drawables = _drawables;
}
