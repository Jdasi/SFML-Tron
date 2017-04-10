#pragma once
#include <queue>

#include "BikeState.h"
#include "Noncopyable.h"

namespace sf
{
    class Packet;
}

#include "MoveDirection.h"
#include "Vector2i.h"

/* Class that represents a player controlled movable object inside the simulation.
 *
 * The Bike itself is non-copyable, but the BikeState data that it uses can be
 * copied for the purposes of synchronising a bike across clients.
 *
 * The Bike's move timer and extra boost timers are handled by the simulation.
 */
class Bike final : public Noncopyable
{
public:
    Bike();
    ~Bike() = default;

    void tick(const double _dt);

    unsigned int getID() const;
    void setID(const unsigned int _id);

    const BikeState& getState() const;
    void overwriteState(const BikeState& _state);

    MoveDirection getDirection() const;
    void setDirection(const MoveDirection _direction);

    Vector2i getPosition() const;
    void setPosition(const Vector2i& _pos);

    const std::vector<Vector2i>& getLine() const;
    void setLine(const std::vector<Vector2i>& _line);
    
    float getMoveSpeed() const;
    void setMoveSpeed(const float _speed);

    double getMoveTimer() const;
    void setMoveTimer(const double _value);
    void decreaseMoveTimer();
    void modifyMoveTimer(const double _dt);

    bool isAlive() const;
    void setAlive(const bool _alive);

    bool isBoosting() const;
    bool activateBoost();
    bool missingBoostCharges() const;
    void grantBoostCharge();
    
    double getExtraBoostTimer() const;
    void resetExtraBoostTimer();
    void modifyExtraBoostTimer(const double _dt);

    bool moveQueueEmpty() const;
    Vector2i getNextQueuedMove();
    void queueMove(const Vector2i& _adjustment);
    void clearMoveQueue();

private:
    BikeState state;

};
