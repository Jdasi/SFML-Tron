#include "Bike.h"
#include "Constants.h"

Bike::Bike()
{
    /* We assume a bike will travel across one tenth of the playing area
     * during its lifetime. If it travels further the vector will expand.
     */
    state.line.reserve(GRID_AREA / 10);
}



// Update the bike's boost timer.
void Bike::tick(const double _dt)
{
    if (state.boost_timer > 0)
    {
        state.boost_timer -= _dt;
    }
    else
    {
        if (state.boosting)
        {
            state.boosting = false;
            state.boost_timer = 0;
        }
    }
}



unsigned int Bike::getID() const
{
    return state.id;
}



void Bike::setID(const unsigned int _id)
{
    state.id = _id;
}



/* Bikes can return a ref to their BikeState because it can only
 * be accessed through a simulation, which returns a copy.
 */
const BikeState& Bike::getState() const
{
    return state;
}



void Bike::overwriteState(const BikeState& _state)
{
    state = _state;
}



MoveDirection Bike::getDirection() const
{
    return state.dir;
}



void Bike::setDirection(const MoveDirection _direction)
{
    state.dir = _direction;
}



Vector2i Bike::getPosition() const
{
    return state.pos;
}



void Bike::setPosition(const Vector2i& _pos)
{
    state.pos = _pos;

    state.line.push_back(_pos);
}



const std::vector<Vector2i>& Bike::getLine() const
{
    return state.line;
}



void Bike::setLine(const std::vector<Vector2i>& _line)
{
    state.line = _line;
}



float Bike::getMoveSpeed() const
{
    if (state.boosting)
    {
        return state.move_speed / BIKE_BOOST_DIVISOR;
    }
    
    return state.move_speed;
}



void Bike::setMoveSpeed(const float _speed)
{
    state.move_speed = _speed;
}



double Bike::getMoveTimer() const
{
    return state.move_timer;
}



void Bike::setMoveTimer(const double _value)
{
    state.move_timer = _value;
}



void Bike::decreaseMoveTimer()
{
    if (state.boosting)
    {
        state.move_timer -= BIKE_MOVE_SPEED / BIKE_BOOST_DIVISOR;
    }
    else
    {
        state.move_timer -= BIKE_MOVE_SPEED;
    }
}



void Bike::modifyMoveTimer(const double _dt)
{
    state.move_timer += _dt;
}



bool Bike::isAlive() const
{
    return state.alive;
}



void Bike::setAlive(const bool _alive)
{
    state.alive = _alive;
}



bool Bike::isBoosting() const
{
    return state.boosting;
}



bool Bike::activateBoost()
{
    if (!state.alive || state.boosting || state.boost_charges <= 0)
    {
        return false;
    }

    state.boosting = true;
    state.boost_timer = BIKE_BOOST_DURATION;
    --state.boost_charges;

    return true;
}



bool Bike::missingBoostCharges() const
{
    return state.boost_charges < STARTING_BOOST_CHARGES;
}



void Bike::grantBoostCharge()
{
    ++state.boost_charges;
}



double Bike::getExtraBoostTimer() const
{
    return state.extra_boost_timer;
}



void Bike::resetExtraBoostTimer()
{
    state.extra_boost_timer = 0;
}



void Bike::modifyExtraBoostTimer(const double _dt)
{
    state.extra_boost_timer += _dt;
}



bool Bike::moveQueueEmpty() const
{
    return state.queued_moves.empty();
}

Vector2i Bike::getNextQueuedMove()
{
    Vector2i next_pos = state.queued_moves.front();
    state.queued_moves.pop();

    return next_pos;
}



void Bike::queueMove(const Vector2i& _adjustment)
{
    state.queued_moves.push(_adjustment);
}



void Bike::clearMoveQueue()
{
    while (!state.queued_moves.empty())
    {
        state.queued_moves.pop();
    }
}
