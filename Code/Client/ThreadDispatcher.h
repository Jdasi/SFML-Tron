#pragma once
#include <Game/Noncopyable.h>

#include <queue>
#include <mutex>
#include <future>

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

    void callEvent(std::function<void()> method)
    {
        std::promise<void> promise;
        std::future<void> future = promise.get_future();

        postEvent([method, &promise]()
        {
            try
            {
                method();
                promise.set_value();
            }
            catch (...)
            {
                promise.set_exception(std::current_exception());
            }
        });

        return future.get();
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
