#pragma once
#include "Noncopyable.h"

template <typename T, typename U>
class StateHandler;

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
