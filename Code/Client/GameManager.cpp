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



/* Updates the countdown timer if it is running, or ticks the simulation
 * once the countdown has finished.
 */
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



// Call when the client's simulation countdown should begin.
void GameManager::startCountdown()
{
    countdown_timer = COUNTDOWN_BEGIN;
    countdown_started = true;
}



// Call when the client's simulation should begin.
void GameManager::startSimulation()
{
    simulation_running = true;
    countdown_started = false;
}



// Call when the client's simulation should stop.
void GameManager::stopSimulation()
{
    simulation_running = false;
    countdown_started = false;
}



// Call when the client's simulation should be reset.
void GameManager::resetSimulation()
{
    stopSimulation();
    simulation.reset();
}



// Used by ClientStateGame to display the countdown.
int GameManager::getCountdownDigit() const
{
    return static_cast<int>(countdown_timer);
}



// Returns true if the simulation is running, otherwise returns false.
bool GameManager::isSimulationRunning() const
{
    return simulation_running;
}



// Returns true if the countdown has started, otherwise returns false.
bool GameManager::hasCountdownStarted() const
{
    return countdown_started;
}



// Convenience method for attaching a SimulationListener to the simulation.
void GameManager::attachSimulationListener(SimulationListener* _listener)
{
    simulation.attachListener(_listener);
}



/* Gets the streamlined network interface of the simulation.
 * This is to avoid exposing certain functionality to other classes
 * that don't own the simulation.
 */
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
