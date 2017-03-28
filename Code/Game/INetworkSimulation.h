#pragma once

class Simulation;
enum MoveDirection;

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

    virtual void changeBikeDirection(unsigned int _bike_id, MoveDirection _dir) = 0;
    virtual void overwrite(const Simulation& _simulation) = 0;
    virtual void overwriteBike(const Bike& _bike) = 0;

};
