#include <Game/Constants.h>
#include "GameManager.h"
#include "ClientData.h"

GameManager::GameManager(ClientData* _client_data)
    : client_data(_client_data)
    , simulation_running(false)
{
}

void GameManager::tick()
{
    if (simulation_running)
    {
        simulation.tick(client_data->delta_time);
    }
}

void GameManager::startSimulation()
{
    simulation_running = true;
}

void GameManager::stopSimulation()
{
    simulation_running = false;

    // do some reset stuff.
}

Simulation* GameManager::getSimulation()
{
    return &simulation;
}

INetworkSimulation* GameManager::getNetworkSimulation()
{
    return &simulation;
}

void GameManager::addPlayer(int _id, PlayerState _state)
{
    if (players.size() >= MAX_PLAYERS)
    {
        return;
    }

    players.emplace(_id, Player(_id, _state));

    if (_id == client_data->client_id)
    {
        players.at(_id).setClient(true);
    }
}

void GameManager::removePlayer(int _id)
{
    players.erase(_id);
}

Player* GameManager::getPlayer(int _id)
{
    return &players.at(_id);
}
