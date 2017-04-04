#pragma once
#include <array>

#include "SimulationListener.h"
#include "ISimulation.h"
#include "Vector2i.h"
#include "Grid.h"
#include "ListenerSubject.h"
#include "Bike.h"
#include "SimulationState.h"
#include "Noncopyable.h"

class Simulation final : public Noncopyable, public ISimulation, 
                         public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(const double _dt);
    void reset();

    Bike& getBike(const unsigned int _bike_id);
    void addBike(const unsigned int _id);
    void removeBike(const unsigned int _bike_id) override;

    void determineOutcome() const;
    unsigned int bikesRemaining() const;
    unsigned int getFirstAliveBikeID() const;

    const Grid& getGrid() const;

    SimulationState getState() const;
    std::array<BikeState, MAX_PLAYERS> getBikes();

    void overwriteState(const SimulationState& _state);
    void changeBikeDirection(const unsigned int _bike_id, const MoveDirection _dir) override;

private:
    void handleBikeMoveTimer(Bike& _bike, const double _dt);
    void handleExtraBoostTimer(Bike& _bike, const double _dt);
    void grantBoostCharge(const unsigned int _bike_id) override;

    void configureBikeSide(Bike& _bike) const;
    void moveBike(Bike& _bike);
    Vector2i generatePositionAdjustment(const MoveDirection _dir, 
        const Vector2i& _current_pos) const;

    bool adjustmentWithinBounds(const Vector2i& _adjustment) const;
    bool adjustmentCollisionCheck(const Vector2i& _adjustment) const;
    bool directionChangeValid(const Bike& _bike, const MoveDirection _new_dir) const;
    bool oppositeDirection(const MoveDirection _lhs, const MoveDirection _rhs) const;

    void resetBikes();
    void overwrite(const SimulationState& _simulation_state) override;
    void overwriteBike(const BikeState& _bike_state) override;
    void overwriteBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;
    void boostBike(const unsigned int _bike_id) override;

    Grid grid;
    std::array<Bike, MAX_PLAYERS> bikes;

};
