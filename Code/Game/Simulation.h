#pragma once
#include <array>

#include "SimulationListener.h"
#include "INetworkSimulation.h"
#include "Vector2i.h"
#include "Grid.h"
#include "ListenerSubject.h"
#include "Bike.h"

namespace sf
{
    class Packet;
}

enum MoveDirection;

class Simulation final : public INetworkSimulation, public ListenerSubject<SimulationListener>
{
public:
    Simulation();
    ~Simulation() = default;

    void tick(double _dt);

    void addBike(unsigned int _id);
    void reset();

    const Grid& getGrid() const;
    std::array<Bike, MAX_PLAYERS>& getBikes();
    Bike& getBike(unsigned int _bike_id);

    void changeBikeDirection(unsigned int _bike_id, MoveDirection _dir) override;

    friend sf::Packet& operator<<(sf::Packet& _packet, Simulation& _simulation);
    friend sf::Packet& operator>>(sf::Packet& _packet, Simulation& _simulation);

private:
    void configureBikeSide(Bike& _bike) const;
    void moveBike(Bike& _bike);
    Vector2i generatePositionAdjustment(MoveDirection _dir, 
        const Vector2i& _current_pos) const;
    void handleBikeDeath(const Bike& _bike);

    bool adjustmentWithinBounds(const Vector2i& _adjustment) const;
    bool adjustmentCollisionCheck(const Vector2i& _adjustment) const;
    bool directionChangeValid(const Bike& _bike, MoveDirection _dir);

    void resetBikes();
    void overwrite(const Simulation& _simulation) override;
    void overwriteBike(const Bike& _bike) override;
    void overwriteBikes(const std::array<Bike, MAX_PLAYERS>& _bikes) override;

    Grid grid;
    std::array<Bike, MAX_PLAYERS> bikes;

};
