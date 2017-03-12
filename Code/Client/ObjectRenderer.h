#pragma once
#include <vector>
#include <memory>

namespace sf
{
    class RenderWindow;
    class Drawable;
}

class ObjectRenderer
{
public:
    ObjectRenderer(sf::RenderWindow& _window);
    ~ObjectRenderer() = default;

    void draw() const;

    void link(std::vector<std::unique_ptr<sf::Drawable>>* _drawables);

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<sf::Drawable>>* drawables;

};
