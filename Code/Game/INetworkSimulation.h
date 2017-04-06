#pragma once
#include "MoveDirection.h"

struct SimulationState;
struct BikeState;

/* Interface class used by TronClient to reveal only certain aspects of
 * the simulation through the GameManager.
 *
 * This provides a simplified interface inside TronClient for dealing
 * with events that should be passed to the simulation for processing.
 */
class INetworkSimulation
{
public:
    INetworkSimulation() = default;
    virtual ~INetworkSimulation() = default;

    virtual void changeBikeDirection(const unsigned int _bike_id, const MoveDirection _dir) = 0;
    virtual void overwriteState(const SimulationState& _simulation_state) = 0;
    virtual void overwriteBike(const BikeState& _bike_state) = 0;
    virtual void overwriteBikes(const std::array<BikeState, MAX_PLAYERS>& _bikes_states) = 0;
    virtual void removeBike(const unsigned int _bike_id) = 0;
    virtual void boostBike(const unsigned int _bike_id) = 0;
    virtual void grantBoostCharge(const unsigned int _bike_id) = 0;

};
