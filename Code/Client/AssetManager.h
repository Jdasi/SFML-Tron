#pragma once
#include <map>
#include <string>
#include <memory>

#include <SFML/Audio.hpp>

namespace sf
{
    class Font;
    class Texture;
}

/* Class which maintains lists of all the assets used by the game.
 * Assets are created when first requested, thereafter a pointer to
 * the asset is returned, to avoid unecessary re-loading.
 */
class AssetManager final
{
public:
    AssetManager() = default;
    ~AssetManager() = default;

    sf::Font* loadFont(const std::string& _file);
    sf::Texture* loadTexture(const std::string& _file);
    sf::SoundBuffer* loadSoundBuffer(const std::string& _file);

private:
    std::map<std::string, std::unique_ptr<sf::Font>> fonts;
    std::map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> buffers;

};
