#pragma once
#include <queue>
#include <mutex>

#include <Game/Noncopyable.h>

/* Class for posting events between classes in a multi-threaded application.
 *
 * Dervied classes must call executeDispatchedMethods() in their main loop
 * to activate any methods that have been posted to them.
 *
 * A copy of the methods queue is made before the methods are iterated 
 * through, meaning it is ok to post events even while the owning thread 
 * is executing its posted events.
 */
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
