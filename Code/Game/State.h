#pragma once
#include "Noncopyable.h"

template <typename T, typename U>
class StateHandler;

/* StateHandler uses a self-referring template pattern.
 * The first templated type indicates which type of state is to be handled,
 * which in essence then decides the second templated type, which forms
 * the type of StateHandler the State is expecting to be handled by.
 *
 * This allows for the State to stay generic, meaning it can be reused for
 * both Client and Server states, which will differ in requirements.
 */
template <typename StateType, typename StateHandlerType>
class State : public Noncopyable
{
template <typename T, typename U>
friend class StateHandler;

public:
    virtual ~State() = default;

    virtual void onStateEnter() = 0;
    virtual void onStateLeave() = 0;

    virtual void tick() = 0;

protected:
    State()
        : handler(nullptr)
    {
    }

    StateHandler<StateType, StateHandlerType>* getHandler() const
    {
        return handler;
    }

private:
    void setHandler(StateHandler<StateType, StateHandlerType>* _handler)
    {
        handler = _handler;
    }

    StateHandler<StateType, StateHandlerType>* handler;

};
