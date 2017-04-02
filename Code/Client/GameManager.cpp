#include <Game/Simulation.h>
#include "GameManager.h"
#include "ClientData.h"
#include "Player.h"

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
    if (simulation_running)
    {
        return;
    }

    simulation_running = true;
}

void GameManager::stopSimulation()
{
    simulation_running = false;

    simulation.reset();
}

void GameManager::attachSimulationListener(SimulationListener* _listener)
{
    simulation.attachListener(_listener);
}

INetworkSimulation* GameManager::getNetworkSimulation()
{
    return &simulation;
}

void GameManager::addPlayer(const unsigned int _id, const PlayerState _state)
{
    players[_id] = std::make_unique<Player>(_id, _state);
}

void GameManager::removePlayer(const unsigned int _id)
{
    players[_id].reset();
}

Player* GameManager::getPlayer(const unsigned int _id)
{
    return players.at(_id).get();
}
