#include <Game/Simulation.h>
#include "GameManager.h"
#include "ClientData.h"
#include "Player.h"

GameManager::GameManager(ClientData* _client_data)
    : client_data(_client_data)
    , simulation_running(false)
    , countdown_started(false)
    , countdown_timer(0)
{
}



void GameManager::tick()
{
    if (countdown_started)
    {
        if (countdown_timer > 0)
        {
            countdown_timer -= client_data->delta_time;
        }
    }

    if (simulation_running)
    {
        simulation.tick(client_data->delta_time);
    }
}



void GameManager::startCountdown()
{
    countdown_timer = COUNTDOWN_BEGIN;
    countdown_started = true;
}



void GameManager::startSimulation()
{
    simulation_running = true;
    countdown_started = false;
}



void GameManager::stopSimulation()
{
    simulation_running = false;
    countdown_started = false;
}



void GameManager::resetSimulation()
{
    stopSimulation();
    simulation.reset();
}



int GameManager::getCountdownDigit() const
{
    return static_cast<int>(countdown_timer);
}



bool GameManager::simulationRunning() const
{
    return simulation_running;
}



void GameManager::attachSimulationListener(SimulationListener* _listener)
{
    simulation.attachListener(_listener);
}



ISimulation* GameManager::getNetworkSimulation()
{
    return &simulation;
}



Player* GameManager::getPlayer(const unsigned int _id)
{
    return players[_id].get();
}



void GameManager::addPlayer(const unsigned int _id, const PlayerState _state)
{
    players[_id] = std::make_unique<Player>(_id, _state);
}



void GameManager::removePlayer(const unsigned int _id)
{
    players[_id].reset();
}
