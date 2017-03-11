#pragma once
#include <vector>
#include <queue>

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

    void draw();

    void unlink(sf::Drawable& _drawable);
    void link(sf::Drawable& _drawable);

private:
    void process_link_queue();
    void process_unlink_queue();

    sf::RenderWindow& window;
    std::vector<sf::Drawable*> drawables;

    std::queue<sf::Drawable*> link_queue;
    std::queue<sf::Drawable*> unlink_queue;

};
