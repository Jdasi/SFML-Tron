#pragma once
#include <string>
#include <memory>
#include <map>

namespace sf
{
    class Sound;
    class Music;
}

class AssetManager;

class GameAudio final
{
public:
    explicit GameAudio(AssetManager* _asset_manager, bool* in_focus);
    ~GameAudio() = default;

    void playSound(const std::string& _file);
    void playMusic(const std::string& _file, const bool _repeating = false);

    void toggleMusicPaused() const;
    void stopMusic();

private:
    AssetManager* asset_manager;
    bool& in_focus;

    std::map<std::string, std::unique_ptr<sf::Sound>> sounds;
    std::unique_ptr<sf::Music> background_music;

};
