#include <SFML/Graphics.hpp>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(sf::RenderWindow& _window)
    : window(_window)
{
}

void ObjectRenderer::draw()
{
    process_link_queue();

    window.clear();
    for (auto& drawable : drawables)
    {
        window.draw(*drawable);
    }
    window.display();
}

void ObjectRenderer::link(sf::Drawable& _drawable)
{
    link_queue.push(&_drawable);
}

void ObjectRenderer::process_link_queue()
{
    while (!link_queue.empty())
    {
        drawables.push_back(link_queue.front());
        link_queue.pop();
    }
}
