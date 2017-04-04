#include <Game/Constants.h>
#include "GameAudio.h"
#include "AssetManager.h"

GameAudio::GameAudio(AssetManager* _asset_manager, bool* in_focus)
    : asset_manager(_asset_manager)
    , in_focus(*in_focus)
{
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



void GameAudio::playMusic(const std::string& _file, const bool _repeating)
{
    if (background_music)
    {
        stopMusic();
    }

    background_music = std::make_unique<sf::Music>();
    if (background_music->openFromFile(AUDIO_PATH + _file))
    {
        background_music->play();
    }

    background_music->setLoop(_repeating);
}



void GameAudio::toggleMusicPaused() const
{
    if (background_music)
    {
        background_music->pause();
    }
}



void GameAudio::stopMusic()
{
    background_music->stop();
    background_music.reset();
}
