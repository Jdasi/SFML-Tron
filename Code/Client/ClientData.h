#pragma once

class AssetManager;
class NetworkManager;
class GameManager;
class InputHandler;

struct ClientData
{
    ClientData(AssetManager* _asset_manager, NetworkManager* _network_manager,
        GameManager* _game_manager, InputHandler* _input_handler)
        : client_id(0)
        , asset_manager(_asset_manager)
        , network_manager(_network_manager)
        , game_manager(_game_manager)
        , input_handler(_input_handler)
        , exit(false)
        , delta_time(0)
        , play_time(0)
        , latency(0)
    {
    }

    int client_id;
    AssetManager* asset_manager;
    NetworkManager* network_manager;
    GameManager* game_manager;
    InputHandler* input_handler;

    bool exit;
    double delta_time;
    double play_time;

    double latency;

};
