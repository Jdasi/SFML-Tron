#include <Game/Constants.h>
#include "GameAudio.h"
#include "AssetManager.h"

GameAudio::GameAudio(AssetManager* _asset_manager, bool* in_focus)
    : asset_manager(_asset_manager)
    , in_focus(*in_focus)
    , music_volume(100.0f)
{
}


void GameAudio::tick(const double _dt)
{
    if (!background_music)
    {
        return;
    }

    background_music->setVolume(in_focus ? music_volume : 0);
}

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



void GameAudio::pauseMusic() const
{
    if (background_music)
    {
        background_music->pause();
    }
}



void GameAudio::resumeMusic() const
{
    if (background_music)
    {
        background_music->play();
    }
}



void GameAudio::stopMusic()
{
    if (!background_music)
    {
        return;
    }

    background_music->stop();
    background_music.reset();
}
