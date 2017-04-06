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

/* Class in which most of the gameplay is generated.
 * The simulation owns an array of Bikes that it moves about the grid
 * when it is allowed to tick.
 *
 * The simulation informs its listeners of various events that occur
 * while the simulation is running, thus allowing for other systems
 * to update their behaviour accordingly.
 */
class Simulation final : public Noncopyable, public INetworkSimulation,
                         public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(const double _dt);
    void reset();

    SimulationState getState() const;
    void overwriteState(const SimulationState& _simulation_state) override;

    BikeState getBikeState(const unsigned int _bike_id) const;
    std::array<BikeState, MAX_PLAYERS> getBikeStates();

    void addBike(const unsigned int _id);
    void removeBike(const unsigned int _bike_id) override;
    bool activateBikeBoost(const unsigned int _bike_id);
    void changeBikeDirection(const unsigned int _bike_id, const MoveDirection _dir) override;

private:
    void determineOutcome() const;
    unsigned int bikesRemaining() const;
    unsigned int getFirstAliveBikeID() const;

    void handleBikeMoveTimer(Bike& _bike, const double _dt);
    void handleExtraBoostTimer(Bike& _bike, const double _dt);
    void grantBoostCharge(const unsigned int _bike_id) override;

    void configureBikeSide(Bike& _bike) const;
    void moveBike(Bike& _bike);
    Vector2i generatePositionAdjustment(const MoveDirection _dir, 
        const Vector2i& _current_pos) const;
    void resolvePositionAdjustment(Bike& _bike, const Vector2i& _adjustment);

    bool adjustmentWithinBounds(const Vector2i& _adjustment) const;
    bool adjustmentCollisionCheck(const Vector2i& _adjustment) const;
    bool directionChangeValid(const Bike& _bike, const MoveDirection _new_dir) const;
    bool oppositeDirection(const MoveDirection _lhs, const MoveDirection _rhs) const;

    void resetBikes();
    void overwriteBike(const BikeState& _bike_state) override;
    void overwriteBikes(const std::array<BikeState, MAX_PLAYERS>& _bike_states) override;
    void boostBike(const unsigned int _bike_id) override;

    Grid grid;
    std::array<Bike, MAX_PLAYERS> bikes;

};
