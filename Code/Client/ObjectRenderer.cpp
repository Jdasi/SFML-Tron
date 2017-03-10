#include <SFML/Graphics.hpp>

#include "ObjectRenderer.h"

ObjectRenderer::ObjectRenderer(sf::RenderWindow& _window)
    : window(_window)
{
    drawables.reserve(100);
}

void ObjectRenderer::draw()
{
    process_unlink_queue();
    process_link_queue();

    window.clear();
    for (auto& drawable : drawables)
    {
        window.draw(*drawable);
    }
    window.display();
}

void ObjectRenderer::unlink(sf::Drawable& _drawable)
{
    unlink_queue.push(&_drawable);
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

void ObjectRenderer::process_unlink_queue()
{
    while (!unlink_queue.empty())
    {
        auto drawable = drawables.front();
        for (auto iter = drawables.begin(); iter != drawables.end(); ++iter)
        {
            if (drawable == (*iter))
            {
                drawables.erase(iter);
                break;
            }
        }

        unlink_queue.pop();
    }
}
