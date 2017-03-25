#pragma once
#include <queue>
#include <mutex>

#include <Game/Noncopyable.h>

class ThreadDispatcher : public Noncopyable
{
public:
    virtual ~ThreadDispatcher() = default;

protected:
    ThreadDispatcher() = default;

    void postEvent(std::function<void()> method)
    {
        std::lock_guard<std::mutex> guard(mutex);
        queue.push(method);
    }

    void executeDispatchedMethods()
    {
        auto queue_copy = getQueueCopy();

        while (!queue_copy.empty())
        {
            auto method = queue_copy.front();
            method();
            queue_copy.pop();
        }
    }

private:
    void clearQueue()
    {
        while (!queue.empty())
        {
            queue.pop();
        }
    }

    std::queue<std::function<void()>> getQueueCopy()
    {
        std::lock_guard<std::mutex> guard(mutex);
        auto queue_copy = queue;
        clearQueue();
        return queue_copy;
    }

    std::queue<std::function<void()>> queue;
    std::mutex mutex;
};
