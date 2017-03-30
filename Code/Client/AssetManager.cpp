#include "AssetManager.h"

sf::Font* AssetManager::loadFontTTF(const std::string& _file)
{
    auto entry = fonts.find(_file);
    if (entry != fonts.end())
    {
        return entry->second.get();
    }

    auto font = std::make_unique<sf::Font>();
    font->loadFromFile("../../Resources/" + _file + ".ttf");
    auto* p_font = font.get();

    fonts[_file] = std::move(font);
    return p_font;
}

sf::Texture* AssetManager::loadTexturePNG(const std::string& _file)
{
    auto entry = textures.find(_file);
    if (entry != textures.end())
    {
        return entry->second.get();
    }

    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromFile("../../Resources/" + _file + ".png");
    auto* p_texture = texture.get();

    textures[_file] = std::move(texture);
    return p_texture;
}
