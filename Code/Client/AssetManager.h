#pragma once
#include <vector>
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
    using Font = std::pair<std::string, std::unique_ptr<sf::Font>>;
    using Texture = std::pair<std::string, std::unique_ptr<sf::Texture>>;
    using SoundBuffer = std::pair<std::string, std::unique_ptr<sf::SoundBuffer>>;

public:
    AssetManager() = default;
    ~AssetManager() = default;

    sf::Font* loadFont(const std::string& _file);
    sf::Texture* loadTexture(const std::string& _file);
    sf::SoundBuffer* loadSoundBuffer(const std::string& _file);

private:
    std::vector<Font> fonts;
    std::vector<Texture> textures;
    std::vector<SoundBuffer> buffers;

};
