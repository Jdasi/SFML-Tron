#pragma once
#include <array>

#include "SimulationListener.h"
#include "INetworkSimulation.h"
#include "Vector2i.h"
#include "Grid.h"
#include "ListenerSubject.h"
#include "Bike.h"
#include "SimulationState.h"
#include "Noncopyable.h"

namespace sf
{
    class Packet;
}

enum MoveDirection;

class Simulation final : public Noncopyable, public INetworkSimulation, 
                         public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(double _dt);
    void reset();

    void addBike(unsigned int _id);
    Bike& getBike(unsigned int _bike_id);
    bool allBikesDead() const;

    const Grid& getGrid() const;

    SimulationState getState() const;
    std::array<BikeState, MAX_PLAYERS> getBikes();

    void overwriteState(const SimulationState& _state);

    void changeBikeDirection(unsigned int _bike_id, const MoveDirection _dir) override;

private:
    void configureBikeSide(Bike& _bike) const;
    void moveBike(Bike& _bike);
    Vector2i generatePositionAdjustment(const MoveDirection _dir, 
        const Vector2i& _current_pos) const;

    bool adjustmentWithinBounds(const Vector2i& _adjustment) const;
    bool adjustmentCollisionCheck(const Vector2i& _adjustment) const;
    bool directionChangeValid(const Bike& _bike, const MoveDirection _new_dir) const;

    void resetBikes();
    void overwrite(const SimulationState& _simulation_state) override;
    void overwriteBike(const BikeState& _bike_state) override;
    void overwriteBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;

    Grid grid;
    std::array<Bike, MAX_PLAYERS> bikes;

};
