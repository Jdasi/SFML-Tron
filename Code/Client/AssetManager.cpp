#include <SFML/Graphics.hpp>

#include <Game/Constants.h>
#include "AssetManager.h"

sf::Font* AssetManager::loadFont(const std::string& _file)
{
    auto entry = fonts.find(_file);
    if (entry != fonts.end())
    {
        return entry->second.get();
    }

    auto font = std::make_unique<sf::Font>();
    font->loadFromFile(FONTS_PATH + _file);
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
    texture->loadFromFile(TEXTURES_PATH + _file);
    auto* p_texture = texture.get();

    textures[_file] = std::move(texture);
    return p_texture;
}



sf::SoundBuffer* AssetManager::loadSoundBuffer(const std::string& _file)
{
    auto entry = buffers.find(_file);
    if (entry != buffers.end())
    {
        return entry->second.get();
    }

    auto sound_buffer = std::make_unique<sf::SoundBuffer>();
    sound_buffer->loadFromFile(AUDIO_PATH + _file);
    auto* p_sound_buffer = sound_buffer.get();

    buffers[_file] = std::move(sound_buffer);
    return p_sound_buffer;
}
