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

    void postEvent(const std::function<void()>& _method);
    void executeDispatchedMethods();

private:
    void clearQueue();
    std::queue<std::function<void()>> getQueueCopy();

    std::queue<std::function<void()>> queue;
    std::mutex mutex;
};
