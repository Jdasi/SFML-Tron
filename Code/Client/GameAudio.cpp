#include <Game/Constants.h>
#include "GameAudio.h"
#include "AssetManager.h"

GameAudio::GameAudio(AssetManager* _asset_manager, bool* in_focus)
    : asset_manager(_asset_manager)
    , in_focus(*in_focus)
    , music_volume(100.0f)
{
}



// Mutes the background music whenever the window has lost focus.
void GameAudio::tick(const double _dt) const
{
    if (!background_music)
    {
        return;
    }

    background_music->setVolume(in_focus ? music_volume : 0);
}



/* Plays a sound from Resources/Audio. Example usage: playSound("my_sound.ogg")
 * A sound is constructed and then played if it has not already been loaded.
 *
 * playSound will abort early if the window is not in focus to avoid sound pollution.
 */
void GameAudio::playSound(const std::string& _file)
{
    if (!in_focus)
    {
        return;
    }

    auto entry = sounds.find(_file);
    if (entry != sounds.end())
    {
        return entry->second->play();
    }

    auto sound = std::make_unique<sf::Sound>();
    sound->setBuffer(*asset_manager->loadSoundBuffer(_file));
    auto* p_sound = sound.get();

    sounds[_file] = std::move(sound);
    p_sound->play();
}



/* Streams music from Resources/Audio. Example usage: playMusic("my_music.ogg")
 * Calling this function while music is playing will cause the current music to stop.
 *
 * Optional params are volume and repeating.
 */
void GameAudio::playMusic(const std::string& _file, const float _volume, 
    const bool _repeating)
{
    if (background_music)
    {
        stopMusic();
    }

    background_music = std::make_unique<sf::Music>();
    background_music->setVolume(_volume);
    background_music->setLoop(_repeating);

    if (background_music->openFromFile(AUDIO_PATH + _file))
    {
        music_volume = _volume;
        background_music->play();
    }
}



// Pauses the current music, if any.
void GameAudio::pauseMusic() const
{
    if (background_music)
    {
        background_music->pause();
    }
}



// Resumes the current music, if any.
void GameAudio::resumeMusic() const
{
    if (background_music)
    {
        background_music->play();
    }
}



// Stops the current music, if any.
void GameAudio::stopMusic()
{
    if (!background_music)
    {
        return;
    }

    background_music->stop();
    background_music.reset();
}
