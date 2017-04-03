#include "AssetManager.h"



sf::Font* AssetManager::loadFont(const std::string& _file)
{
    auto entry = fonts.find(_file);
    if (entry != fonts.end())
    {
        return entry->second.get();
    }

    auto font = std::make_unique<sf::Font>();
    font->loadFromFile("Resources/Fonts/" + _file);
    auto* p_font = font.get();

    fonts[_file] = std::move(font);
    return p_font;
}



sf::Texture* AssetManager::loadTexture(const std::string& _file)
{
    auto entry = textures.find(_file);
    if (entry != textures.end())
    {
        return entry->second.get();
    }

    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromFile("Resources/Images/" + _file);
    auto* p_texture = texture.get();

    textures[_file] = std::move(texture);
    return p_texture;
}
