#pragma once
#include <string>

class AssetManager;
class NetworkManager;
class GameManager;
class InputHandler;
class GameAudio;

struct ClientData
{
    ClientData(AssetManager* _asset_manager, NetworkManager* _network_manager,
        GameManager* _game_manager, InputHandler* _input_handler, GameAudio* _game_audio)
        : asset_manager(_asset_manager)
        , network_manager(_network_manager)
        , game_manager(_game_manager)
        , input_handler(_input_handler)
        , game_audio(_game_audio)
        , exit(false)
        , delta_time(0)
        , play_time(0)
        , server_bulletin_str("Connecting...")
        , latency(0)
        , client_id(0)
        , victor_id(0)
    {
    }

    AssetManager* asset_manager;
    NetworkManager* network_manager;
    GameManager* game_manager;
    InputHandler* input_handler;
    GameAudio* game_audio;

    bool exit;
    double delta_time;
    double play_time;

    std::string server_bulletin_str;
    double latency;

    unsigned int client_id;
    unsigned int victor_id;

};
