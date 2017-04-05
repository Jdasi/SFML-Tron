#pragma once
#include <array>

#include <Game/Constants.h>

struct BikeState;
struct SimulationState;

/* Interface class used by SimulationThread to decouple the simulation from
 * the TronServer.
 *
 * This provides a simplified interface inside the SimulationThread for dealing
 * with events that should be passed to the server for processing.
 */
class IServerSimulation
{
public:
    IServerSimulation() = default;
    virtual ~IServerSimulation() = default;

    virtual void onSyncSimulation(const SimulationState& _simulation) = 0;
    virtual void onSyncBike(const BikeState& _bike) = 0;
    virtual void onSyncAllBikes(const std::array<BikeState, MAX_PLAYERS>& _bikes) = 0;
    virtual void onBikeRemoved(const unsigned int _bike_id) = 0;
    virtual void onBikeBoost(const unsigned int _bike_id) = 0;
    virtual void onBoostChargeGranted(const unsigned int _bike_id) = 0;

    virtual void onSimulationStarted() = 0;
    virtual void onSimulationStopping() = 0;
    virtual void onSimulationEnded() = 0;
    virtual void onSimulationReset() = 0;
    virtual void onSimulationVictor(const unsigned int _bike_id) = 0;

};
