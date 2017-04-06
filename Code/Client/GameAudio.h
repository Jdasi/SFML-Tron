#pragma once
#include <string>
#include <memory>
#include <vector>

namespace sf
{
    class Sound;
    class Music;
}

class AssetManager;

/* Class for playing sounds and music.
 * GameAudio communicates with the AssetManager to populate its map of Sounds.
 * Music is streamed from directly from a file in accordance with SFML guidance.
 *
 * GameAudio will mute music and ignore playSound calls if the window is not in focus.
 */
class GameAudio final
{
using Sound = std::pair<std::string, std::unique_ptr<sf::Sound>>;

public:
    explicit GameAudio(AssetManager* _asset_manager, bool* in_focus);
    ~GameAudio() = default;

    void tick(const double _dt) const;

    void playSound(const std::string& _file);
    void playMusic(const std::string& _file, const float _volume = 100.0f, 
        const bool _repeating = false);

    void pauseMusic() const;
    void resumeMusic() const;
    void stopMusic();

private:
    AssetManager* asset_manager;
    bool& in_focus;

    std::vector<Sound> sounds;
    std::unique_ptr<sf::Music> background_music;
    float music_volume;

};
