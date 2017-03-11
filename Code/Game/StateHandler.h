#pragma once
#include <map>
#include <memory>
#include <queue>
#include <mutex>

#include "Noncopyable.h"

/* StateHandler uses a self-referring template pattern.
 * The first templated type indicates which type of state is to be handled,
 * which in essence then decides the second templated type, which forms
 * the type of StateHandler is created.
 *
 * This allows for the StateHandler to stay generic, meaning it can be reused for
 * both Client and Server state handlers, which will differ in requirements.
 */
template <typename StateType, typename StateHandlerType>
class StateHandler : public Noncopyable
{
public:
    StateHandler()
        : current_state(nullptr)
    {
    }

    virtual ~StateHandler() = default;

    void registerState(const std::string& _key, std::unique_ptr<StateType> _state)
    {
        _state->setHandler(this);
        states[_key] = std::move(_state);
    }

    void queueState(const std::string& _key)
    {
        std::lock_guard<std::mutex> guard(states_queue_mutex);
        states_queue.push(_key);
    }

    void tick()
    {
        processStatesQueue();

        if (current_state)
        {
            current_state->tick();
        }
    }

protected:
    StateType* current_state;

private:
    void processStatesQueue()
    {
        std::lock_guard<std::mutex> guard(states_queue_mutex);
        if (!states_queue.empty())
        {
            triggerState(states_queue.front());
            states_queue.pop();
        }
    }

    void triggerState(const std::string& _key)
    {
        auto result = states.find(_key);

        if (result == states.end())
        {
            throw std::runtime_error("Unknown state.");
        }

        if (current_state)
        {
            current_state->onStateLeave();
        }

        current_state = result->second.get();

        current_state->onStateEnter();
    }

    std::map<std::string, std::unique_ptr<StateType>> states;

    std::queue<std::string> states_queue;
    std::mutex states_queue_mutex;

};
