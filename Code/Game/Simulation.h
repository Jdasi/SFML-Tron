#pragma once
#include <vector>

#include "SimulationListener.h"
#include "INetworkSimulation.h"
#include "Vector2i.h"
#include "Grid.h"
#include "ListenerSubject.h"
#include "Bike.h"

enum MoveDirection;

class Simulation final : public INetworkSimulation, public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(double _dt);

    void addBike();
    
    // INetworkSimulation methods.
    void changeBikeDirection(unsigned int _bike_id, MoveDirection _dir) override;

private:
    void moveBike(Bike& _bike);
    Vector2i generatePositionAdjustment(MoveDirection _dir, Vector2i _current_pos) const;
    bool adjustmentWithinBounds(Vector2i _adjustment) const;
    bool adjustmentCollisionCheck(Vector2i _adjustment) const;
    bool directionChangeValid(Bike& _bike, MoveDirection _dir);

public:
    Grid grid;
    std::vector<Bike> bikes;
    int colours_assigned;

};
